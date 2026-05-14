## 🧪 Test Directory

此目录存放项目的示例 `main` 函数，通过替换根目录的 `main.cpp` 体验不同功能。所有示例共用同一套网络库代码，仅 `main.cpp` 不同。

### 📦 示例列表

| 示例 | 功能 | 使用方式 |
| :--- | :--- | :--- |
| 基础 HTTP | 返回 `Hello`，验证框架能跑通 | `cp test/basic.cpp main.cpp && make` |
| JSON API | 根据 URL 路径返回不同 JSON 数据 | `cp test/json_api.cpp main.cpp && make` |
| 静态页面 | 返回完整 HTML 网页 | `cp test/html_page.cpp main.cpp && make` |
| IoT 控制 | 模拟智能家居设备控制 | `cp test/iot_control.cpp main.cpp && make` |
| 回声服务 | 收到什么就返回什么 | `cp test/echo.cpp main.cpp && make` |

### 🧪 测试方法

以 JSON API 为例：

```bash
cp test/json_api.cpp main.cpp
make
./http_server

# 另开终端
curl http://localhost:8080/api/status
# {"status":"ok","uptime":3600}

curl http://localhost:8080/api/version
# {"version":"1.0.0","author":"hk"}
```

### 💡 小提示

替换 `main.cpp` 后需重新编译。如果使用 CMake，替换后直接 `make` 即可，CMake 会自动检测文件变化。
