```markdown
## 📁 Include Directory

此目录存放项目的所有头文件（`.h`），定义各模块的接口与类型声明。

### 📦 模块一览

| 头文件 | 所属模块 | 功能描述 |
| :--- | :--- | :--- |
| `mem_pool.h` | FixedMemPool | 固定大小内存分配器，空闲链表实现，O(1) 分配回收 |
| `thread_pool.h` | ThreadPool | 固定线程数任务线程池，条件变量驱动，支持并发执行 |
| `buffer.h` | Buffer | 读写缓冲区，自动扩容，支持 `\r\n` 行尾查找 |
| `tcp_connection.h` | TcpConnection | TCP 连接封装，回调驱动，读写缓冲管理 |
| `event_loop.h` | EventLoop | 事件循环引擎，基于 select 多路复用，支持可读可写事件分发 |
| `acceptor.h` | Acceptor | 端口监听与连接接收，新连接回调通知 |

### 📐 依赖关系

```
mem_pool.h
    ↓
buffer.h ←── tcp_connection.h ←── event_loop.h ←── acceptor.h
                                     ↑
                               thread_pool.h
```

### 📝 编码规范

- 使用 `#pragma once` 防止重复包含
- 类名采用 PascalCase（如 `TcpConnection`）
- 成员变量命名后缀 `_`（如 `readPos_`、`fd_`）
- 公开接口添加 `const` 修饰，保证只读不写

---

> 💡 头文件只做声明，实现细节在 `src/` 目录对应的 `.cpp` 文件中。
```
