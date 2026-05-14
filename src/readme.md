```markdown
## 📁 Src Directory

此目录存放项目的所有源文件（`.cpp`），实现各模块的核心逻辑。

### 📦 模块一览

| 源文件 | 所属模块 | 功能描述 |
| :--- | :--- | :--- |
| `mem_pool.cpp` | FixedMemPool | 固定大小内存分配器，基于空闲链表，O(1) 分配回收，无内存碎片 |
| `thread_pool.cpp` | ThreadPool | 固定线程数任务队列，条件变量实现阻塞唤醒，支持优雅关闭 |
| `buffer.cpp` | Buffer | 读写缓冲区实现，自动扩容与收缩，内建 `\r\n` 行尾查找 |
| `tcp_connection.cpp` | TcpConnection | TCP 连接管理，`recv` 数据暂存读 Buffer，`send` 数据先缓存到写 Buffer |
| `event_loop.cpp` | EventLoop | 事件循环引擎，`select` 监听可读可写事件，线程池并发处理 |
| `acceptor.cpp` | Acceptor | 端口绑定与监听，`accept` 新连接并回调通知上层 |

### 🧩 模块关系

```
mem_pool.cpp        ← Buffer 底层内存分配
thread_pool.cpp     ← EventLoop 并发任务处理

buffer.cpp          ← TcpConnection 收发数据暂存
tcp_connection.cpp  ← EventLoop 事件分发目标
event_loop.cpp      ← Acceptor 新连接加入事件循环
acceptor.cpp        ← main.cpp 服务启动入口
```

### 🔧 平台适配

```cpp
#ifdef _WIN32
    #include <winsock2.h>   // Windows Socket API
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>  // Linux/ARM Socket API
    #include <unistd.h>
#endif
```

一套代码，Windows 和 Linux/ARM 均可编译运行。

### 📝 编码规范

- C++17 标准，零第三方库依赖
- 系统调用返回值必须检查（`recv`、`send`、`select`）
- 网络 I/O 错误时关闭连接并设置 `fd_ = -1`，防止重复关闭
- 构造与析构成对管理资源（`new` / `delete`，`socket` / `close`）

---

> 💡 每个 `.cpp` 文件对应 `include/` 目录下同名的 `.h` 头文件。
```
