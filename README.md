# lightweight_server

![C++](https://img.shields.io/badge/C%2B%2B-17-blue)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-green)
![Dependencies](https://img.shields.io/badge/第三方网络库-无-red)

一个用于学习 C++ Socket 编程和服务器模块协作的轻量级 TCP/HTTP 示例。项目包含固定块内存池、线程池、缓冲区、TCP 连接、基于 `select` 的事件循环和连接接收器。服务器监听 `8080` 端口，收到客户端数据后按 `\r\n` 提取一行，并返回固定的 `Hello` HTTP 响应。

> 当前项目是学习型示例，不是完整 HTTP 实现。它没有完整解析请求头或请求体，也没有实现生产环境所需的连接管理和优雅退出。

## 核心模块

| 模块 | 文件 | 职责 |
| :--- | :--- | :--- |
| `FixedMemPool` | `include/mem_pool.h`、`src/mem_pool.cpp` | 预分配固定大小内存块，并用空闲链表分配和回收。 |
| `ThreadPool` | `include/thread_pool.h`、`src/thread_pool.cpp` | 创建固定数量的工作线程；使用任务队列和条件变量分发任务。 |
| `Buffer` | `include/buffer.h`、`src/buffer.cpp` | 暂存接收和待发送的数据，支持扩容、读取和查找 `\r\n`。 |
| `TcpConnection` | `include/tcp_connection.h`、`src/tcp_connection.cpp` | 管理单个客户端 socket、读写缓冲区和读回调。 |
| `EventLoop` | `include/event_loop.h`、`src/event_loop.cpp` | 使用 `select` 监视连接的读写状态，并将处理任务交给线程池。 |
| `Acceptor` | `include/acceptor.h`、`src/acceptor.cpp` | 绑定并监听端口，接受客户端连接后创建 `TcpConnection`。 |

## 工作流程

```text
Acceptor: socket -> bind -> listen -> accept
                                  |
                                  v
                         创建 TcpConnection
                                  |
                                  v
EventLoop: select 检查连接的可读/可写状态
          | 可读                         | 可写
          v                              v
ThreadPool 执行 handleRead()       ThreadPool 执行 handleWrite()
          |                              ^
          v                              |
recv() -> read Buffer -> 读回调 -> send() -> write Buffer
```

`main.cpp` 设置新连接回调和数据读取回调。`Acceptor` 在单独线程中等待新客户端；主线程运行 `EventLoop`；事件循环创建的 4 个工作线程执行连接读写任务。当前 `Buffer` 在读缓冲区中查找第一个 `\r\n`，找到后将这一行交给回调；示例回调返回固定响应：

```http
HTTP/1.1 200 OK
Content-Length: 5

Hello
```

## 快速开始

需要支持 C++17 的编译器。项目使用操作系统 Socket API 和 C++ 标准库，没有第三方网络库依赖。

### Linux

```bash
mkdir -p build
g++ -std=c++17 -Wall -Wextra -pthread -Iinclude \
    main.cpp \
    src/acceptor.cpp src/buffer.cpp src/event_loop.cpp \
    src/mem_pool.cpp src/tcp_connection.cpp src/thread_pool.cpp \
    -o build/http_server
./build/http_server
```

### Windows（MinGW / MSYS2）

```bash
mkdir -p build
g++ -std=c++17 -Wall -Wextra -pthread -Iinclude \
    main.cpp \
    src/acceptor.cpp src/buffer.cpp src/event_loop.cpp \
    src/mem_pool.cpp src/tcp_connection.cpp src/thread_pool.cpp \
    -lws2_32 -o build/http_server.exe
./build/http_server.exe
```

服务启动后，在浏览器访问 `http://127.0.0.1:8080/`，或运行：

```bash
curl -i http://127.0.0.1:8080/
```

正常情况下响应正文为 `Hello`。程序当前持续运行，停止时使用终端中断信号。

## 当前限制

- `\r\n` 只用于识别一行结束，不代表完整 HTTP 请求已接收；当前没有解析请求头和请求体。
- 响应内容固定为 `Hello`，没有路由、JSON API 或静态文件服务。
- `select` 适合演示 I/O 多路复用，连接规模增大时存在扩展限制。
- 同一连接的读写任务由工作线程执行；连接缓冲区的并发访问尚未完整串行化。
- 固定内存池的空闲链表没有线程同步，也没有检查重复释放或指针是否属于该内存池。
- 连接移除、线程停止和优雅关闭流程尚未完整实现。
- 源码包含 Windows 和 POSIX Socket 分支；当前构建说明面向 MinGW/MSYS2 和 Linux，ARM Linux 尚未单独验证。

## 目录结构

```text
.
├── include/       # 头文件
├── src/           # 模块实现
├── main.cpp       # 程序入口和回调组装
└── README.md
```
