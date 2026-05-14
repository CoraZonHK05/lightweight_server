## 🚀 Build Directory

此目录存放编译后的可执行文件和相关构建产物。

### 📦 当前版本

| 平台 | 可执行文件 | 状态 |
| :--- | :--- | :--- |
| Windows | `http_server.exe` | ✅ Ready |
| Linux/ARM | `http_server` | ✅ Ready |

### 🔨 编译命令

#### CMake（推荐）

```bash
mkdir build && cd build
cmake ..
make
```

#### 手动 g++

<details>
<summary>🪟 Windows</summary>

```
g++ -std=c++17 -I../include \
    ../src/mem_pool.cpp \
    ../src/thread_pool.cpp \
    ../src/buffer.cpp \
    ../src/tcp_connection.cpp \
    ../src/event_loop.cpp \
    ../src/acceptor.cpp \
    ../main.cpp \
    -o http_server \
    -lws2_32
```
</details>

<details>
<summary>🐧 Linux / ARM</summary>

```
g++ -std=c++17 -I../include \
    ../src/mem_pool.cpp \
    ../src/thread_pool.cpp \
    ../src/buffer.cpp \
    ../src/tcp_connection.cpp \
    ../src/event_loop.cpp \
    ../src/acceptor.cpp \
    ../main.cpp \
    -o http_server \
    -pthread
```
</details>

### 📂 目录结构

```
+ http_server          # 可执行文件（Linux/ARM）
+ http_server.exe      # 可执行文件（Windows）
```

### 🧹 清理

```
# 删除所有编译产物
rm -rf *
```

---

> ⚠️ 此目录下的 `.exe` 和二进制文件**不会**被 Git 追踪（已配置 `.gitignore`）
