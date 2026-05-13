# lightweight_server

![C++](https://img.shields.io/badge/C%2B%2B-17-blue)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20ARM-green)
![License](https://img.shields.io/badge/License-MIT-yellow)
![Lines](https://img.shields.io/badge/代码量-~600行-orange)
![Dependencies](https://img.shields.io/badge/依赖-零第三方库-red)

轻量级 C++ 网络库，包含内存池、线程池、事件循环和 HTTP 服务模块。纯手写，零第三方网络库依赖，约 600 行代码。支持 Windows、Linux 和 ARM Linux 跨平台部署。

## 核心模块

| 模块 | 文件 | 职责 |
| :--- | :--- | :--- |
| FixedMemPool | `include/mem_pool.h` | 固定大小内存分配器，基于空闲链表，O(1) 分配回收，无内存碎片 |
| ThreadPool | `include/thread_pool.h` | 固定线程数的任务线程池，条件变量驱动，支持阻塞等待与唤醒 |
| Buffer | `include/buffer.h` | 自动扩容的读写缓冲区，支持 CRLF 行尾查找，用于 HTTP 协议解析 |
| TcpConnection | `include/tcp_connection.h` | 管理单个 TCP 连接，回调驱动，读写缓冲分离 |
| EventLoop | `include/event_loop.h` | 基于 select I/O 复用的事件循环，单线程管理多个并发连接 |
| Acceptor | `include/acceptor.h` | 监听端口，接受新连接，通过回调通知上层 |

## 架构

`main` 函数启动后，由 `Acceptor` 监听端口并接受新连接。每个新连接被封装为一个 `TcpConnection` 对象，注册到 `EventLoop` 中。

`EventLoop` 基于 `select` 实现事件循环，监听所有连接的可读和可写事件：
- 可读事件触发 `TcpConnection::handleRead()`，从 socket 接收数据写入 `Buffer`，当 `Buffer` 中检测到完整的 HTTP 请求行后，调用用户设置的回调函数进行处理。
- 可写事件触发 `TcpConnection::handleWrite()`，将 `Buffer` 中待发送的数据通过 socket 发出。

`EventLoop` 内部使用 `ThreadPool` 提交读写任务，由工作线程并发执行回调，避免阻塞主事件循环。

## 快速开始

### 编译

```bash
# Linux / ARM Linux
g++ -std=c++17 -I./include \
    ./src/mem_pool.cpp \
    ./src/thread_pool.cpp \
    ./src/buffer.cpp \
    ./src/tcp_connection.cpp \
    ./src/event_loop.cpp \
    ./src/acceptor.cpp \
    ./main.cpp \
    -o ./build/http_server -pthread

# Windows (MinGW / MSYS2)
g++ -std=c++17 -I./include \
    ./src/mem_pool.cpp \
    ./src/thread_pool.cpp \
    ./src/buffer.cpp \
    ./src/tcp_connection.cpp \
    ./src/event_loop.cpp \
    ./src/acceptor.cpp \
    ./main.cpp \
    -o ./build/http_server -lws2_32
