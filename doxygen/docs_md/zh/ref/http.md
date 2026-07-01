@page http HTTP 模块参考 {#http}

[TOC]

# HTTP 模块参考 — HttpClient / HttpPoller / HttpTypes / Headers

---

## HttpClient — HTTP 客户端 {#http-client}

`dao::web::HttpClient` 提供统一的 HTTP/HTTPS 客户端，支持同步和异步请求，内部线程池管理。

@ref http "→ 返回教程：HTTP 客户端"

### 头文件

```cpp
#include <web/http/HttpClient.hpp>
```

### 构造

```cpp
// 直接构造（port==443 时自动启用 SSL）
explicit HttpClient(const std::string &host, int port = 80);

// 从 URL 构造（根据 scheme 自动选择）
static HttpClient fromURL(const std::string &url);

// 显式 HTTPS
static HttpClient https(const std::string &host, int port = 443);
```

### 同步请求（阻塞）

```cpp
// GET
HttpResponse get(const std::string &path) const;

// 通用请求
HttpResponse request(const HttpRequest &req) const;
```

### 异步请求（回调式，线程池执行）

```cpp
// 异步 GET
void get(const std::string &path, HttpCallback callback);

// 异步 POST
void post(const std::string &path, const std::string &body,
          HttpCallback callback,
          const std::string &content_type = "application/json");

// 带自定义请求头的异步 POST
void post(const std::string &path, const Headers &headers,
          const std::string &body, HttpCallback callback,
          const std::string &content_type = "application/json");

// 通用异步请求
void request(const HttpRequest &req, HttpCallback callback);
```

### 主线程回调投递

异步回调默认在线程池线程执行，不能直接操作 SDL UI。通过 `setMainThreadCallbackHandler` 投递到主线程：

```cpp
void setMainThreadCallbackHandler(
    std::function<void(std::function<void()>)> handler);
```

`handler` 接收一个 `function<void()>`，由调用方决定何时在何线程执行。典型实现：存入队列，在 `update()` 中取出执行。

### SSL 配置

```cpp
void setCaCertPath(const std::string &ca_cert_path) const;
void enableSSLVerification(bool enable) const;  // 默认禁用
```

### 超时设置

```cpp
void setTimeout(int connection_sec, int read_sec) const;
```

### 信息查询

```cpp
const std::string &getHost() const;
int getPort() const;
bool isHttps() const;
```

---

## HttpPoller — 定时轮询器 {#http-poller}

`HttpPoller` 封装定时轮询逻辑，在 `update()` 中调用 `tick()`，到达间隔时自动发起异步 GET。

@ref http-poller "→ 返回教程：HttpPoller"

### 头文件

```cpp
#include <web/http/HttpPoller.hpp>
```

### 构造函数

```cpp
HttpPoller(HttpClient &client, HttpRequest req,
           std::chrono::seconds interval);
```

| 参数 | 说明 |
|------|------|
| `client` | HttpClient 引用（生命周期必须长于 HttpPoller） |
| `req` | 请求模板 |
| `interval` | 轮询间隔 |

### 方法

| 方法 | 说明 |
|------|------|
| `tick()` | 每帧调用，到达间隔时自动发起异步请求 |
| `hasResponse()` | 是否有已完成的响应待消费 |
| `takeResponse()` | 取出响应（消费，取出后内部清除） |
| `setInterval(interval)` | 修改轮询间隔 |
| `setRequest(req)` | 修改请求模板 |

### 使用模式

```cpp
class MyPage : public dao::ifc::IGeneralPage {
    dao::web::HttpClient m_client{"api.example.com"};
    dao::web::HttpPoller m_poller{m_client,
        dao::web::HttpRequest{.path = "/syncData"},
        std::chrono::seconds(10)};

public:
    void update() override {
        m_poller.tick();
        while (m_poller.hasResponse()) {
            auto resp = m_poller.takeResponse();
            // 处理响应...
        }
        addToBatch(/* ... */);
    }
};
```

---

## HttpTypes — 类型定义 {#http-types}

### 头文件

```cpp
#include <web/http/HttpTypes.hpp>
```

### HttpMethod 枚举

```cpp
enum class HttpMethod : i8 { Get, Post, Put, Delete, Patch };
```

### HttpResponse

```cpp
struct HttpResponse {
    int status = 0;
    std::string body;
    std::string error;
    bool success = false;   // status == 200
};
```

### HttpRequest

```cpp
struct HttpRequest {
    HttpMethod method = HttpMethod::Get;
    std::string path;
    Headers headers;
    std::string body;
    std::string content_type = "application/json";
    i32 connect_timeout_sec = 5;
    i32 read_timeout_sec = 10;
};
```

### HttpCallback

```cpp
using HttpCallback = std::function<void(HttpResponse)>;
```

### ParsedURL / parseHost

```cpp
struct ParsedURL {
    std::string host;
    i32 port = 80;
    std::string path = "/";
    bool isHttps = false;
};

ParsedURL parseHost(const std::string &url);
```

---

## Headers — 请求头 {#http-headers}

### 头文件

```cpp
#include <web/http/Headers.hpp>
```

### 构造

```cpp
Headers() = default;

// 初始化列表
Headers(std::initializer_list<std::pair<std::string, std::string>> init);
```

### 方法

| 方法 | 说明 |
|------|------|
| `add(key, value)` | 添加头（允许同名） |
| `set(key, value)` | 设置头（覆盖同名） |
| `get(key)` | 获取头值 |
| `remove(key)` | 删除头 |
| `clear()` | 清空全部 |
| `empty()` | 是否为空 |
| `size()` | 头的数量 |

### 示例

```cpp
dao::web::Headers headers = {
    {"Content-Type", "application/json"},
    {"Authorization", "Bearer token123"}
};
headers.add("X-Custom", "value");
std::string auth = headers.get("Authorization");
```