@page tutorial DaoLib 入门教程

[TOC]

# DaoLib 从零到精通

## 1. 简介 {#intro}

DaoLib 是一个基于 SDL3 的跨平台 C++ GUI 开发框架，使用 C++23 标准，支持 Windows、macOS、Linux。它提供了完整的应用管理、窗口系统、页面组件、几何图形渲染、HTTP 客户端、SQLite 数据库等功能。

**核心特性：**

| 特性 | 说明 |
|------|------|
| 跨平台 | 基于 SDL3，一套代码多平台运行 |
| 现代 C++ | C++23，Concept 约束模板，编译期计算 |
| 页面管理 | 多页面切换，生命周期清晰 |
| 几何图形 | 9 种内置图形组件，支持平移旋转 |
| 批处理渲染 | 零散图元自动合并为批次，减少 GPU 调用 |
| HTTP 客户端 | 同步/异步请求，自动线程池，主线程回调投递 |
| SQLite | 类型安全查询，自动列名映射 |
| 系统托盘 | 托盘图标 + 自定义菜单 |

---

## 2. 环境搭建 {#setup}

### 2.1 系统要求

- **编译器**：GCC 13+ / Clang 16+ / MSVC 2022 17.8+
- **CMake**：3.31+
- **依赖**：SDL3、SDL3_image、SDL3_ttf、utf8cpp、SQLiteCpp、nlohmann/json、cpp-httplib

### 2.2 安装依赖

DaoLib 使用 vcpkg 管理依赖：

```bash
git clone https://github.com/microsoft/vcpkg.git ~/.vcpkg-clion/vcpkg
~/.vcpkg-clion/vcpkg/bootstrap-vcpkg.sh
```

项目根目录的 `vcpkg.json` 已声明所有依赖，配合 CMake 工具链自动安装。

### 2.3 构建 DaoLib

```bash
# 构建 Debug + Release（增量）
./build-install.sh

# 仅构建 Release
./build-install.sh Release

# 清理后重建
./build-install.sh Release 1
```

产物输出到 `install/Debug/` 和 `install/Release/`。

### 2.4 在项目中使用 DaoLib

```cmake
cmake -S . -B build \
  -DCMAKE_PREFIX_PATH="/path/to/DaoLib/install/Release"

# CMakeLists.txt
find_package(dao REQUIRED)
target_link_libraries(myapp PRIVATE dao::dao)
```

---

## 3. 快速开始 {#quickstart}

### 3.1 最小应用

```cpp
#include <core/frame/App.hpp>

int main() {
    dao::App app{60};                              // 60 FPS
    app.createWindow(800, 600, "main",             // 800×600，标签 "main"
                     dao::Window::WorkState::Foreground);
    app.run();
}
```

这段代码创建一个 800×600 的空白窗口并以 60 FPS 运行。

### 3.2 Hello World — 第一个带页面的应用

```cpp
#include <core/frame/App.hpp>
#include <interface/IGeneralPage.hpp>
#include <components/Text.hpp>

class HelloPage : public dao::ifc::IGeneralPage {
    dao::Text m_text{100, 100, 32, dao::hexToRGBA("#333333"), U"你好，DaoLib！"};

public:
    HelloPage() : IGeneralPage("hello") {}

    std::vector<dao::i32> getRegisterTextures() const override { return {}; }
    void open() override {}
    void close() override {}

    void update() override {
        addToBatch(m_text);   // 将文本加入渲染批次
    }

    void handleInputEvent(const SDL_Event &event) override {}
};

int main() {
    dao::GlyphAtlas::setTtfPath("./assets/ttf/zh-cn.ttf");  // 设置字体

    dao::App app{60};
    app.createWindow(800, 600, "main",
                     dao::Window::WorkState::Foreground)
        .addPage<HelloPage>();
    app.run();
}
```

关键点：
- 继承 `dao::ifc::IGeneralPage`，构造函数传入页面标题
- `getRegisterTextures()` 返回页面需要的纹理 ID 列表
- `update()` 每帧调用，用 `addToBatch()` 将组件送入渲染
- `handleInputEvent()` 处理 SDL 事件（鼠标、键盘等）
- `dao::GlyphAtlas::setTtfPath()` 必须在创建窗口前调用

---

## 4. 核心概念 {#core-concepts}

### 4.1 App — 应用程序入口 {#app}

`dao::App` 管理整个应用的生命周期。每个应用只有一个 App 实例。

@ref app "→ App 参考文档"

```cpp
// 构造函数：帧率（默认60）、失焦点击是否穿透（默认false）
dao::App app{60, false};

// 创建窗口（详见 §4.2）
Window &win = app.createWindow(800, 600, "main", Window::WorkState::Foreground,
                                true /*isSubject*/, true /*resizable*/);

// 创建托盘
app.createTray<MyTray>();

// 显示/隐藏指定标签的窗口
app.showWindow("settings");
app.hideWindow("settings");

// 启动主循环（阻塞直到 exit()）
app.run();

// 退出应用
app.exit();

// 获取全局上下文
Context &ctx = app.getContext();
```

### 4.2 Window — 窗口管理 {#window}

`dao::Window` 代表一个应用窗口，可以包含多个页面。

@ref window "→ Window 参考文档"

```cpp
Window &createWindow(
    i32 width, i32 height,           // 窗口尺寸
    std::string_view tag,            // 唯一标识（用于查找）
    Window::WorkState workState,     // 初始工作状态
    bool isSubject = false,          // 是否为主体窗口（关闭时退出应用）
    bool resizable = false,          // 是否可调整大小
    bool transparent = false,        // 是否支持透明
    bool onTop = false,              // 是否置顶
    bool borderless = false          // 是否无边框
);
```

**WorkState 枚举：**

| 值 | 含义 |
|----|------|
| `Foreground` | 前台运行（正常显示） |
| `Background` | 后台运行（隐藏但仍在更新） |
| `Closed` | 窗口关闭 |

**窗口操作：**

```cpp
// 添加页面
window.addPage<MyPage>();                 // 模板版本（推荐）
window.addPage(std::make_unique<MyPage>()); // unique_ptr 版本

// 切换页面
window.switchPage("pageTitle");

// 控制窗口
window.show();
window.hide();
window.setPosition(100, 100);
window.setSize(1024, 768);
window.setTitle("新标题");
window.setClickThrough(true);  // 鼠标事件穿透

// 转换工作状态
window.convertWorkState(Window::WorkState::Background);
```

### 4.3 IGeneralPage — 页面接口 {#page}

页面是 UI 内容的容器。所有自定义页面都应继承 `dao::ifc::IGeneralPage`。

@ref page "→ IGeneralPage 参考文档"

```cpp
class MyPage : public dao::ifc::IGeneralPage {
public:
    MyPage() : IGeneralPage("页面标题") {}

    // 返回页面需要的纹理 ID 列表（页面打开前调用）
    std::vector<dao::i32> getRegisterTextures() const override {
        return {TEXTURE_ID_1, TEXTURE_ID_2};
    }

    // 页面打开时调用（初始化资源）
    void open() override { /* ... */ }

    // 页面关闭时调用（释放资源）
    void close() override { /* ... */ }

    // 每帧调用（更新逻辑 + 渲染）
    void update() override {
        addToBatch(component1, component2, text);
    }

    // 处理输入事件（鼠标、键盘等）
    void handleInputEvent(const SDL_Event &event) override {
        button.handleEvent(event);
    }
};
```

**页面生命周期：**

```
创建页面 → getRegisterTextures() → open() → [update() × N] → close() → 销毁
                ↑                                        ↑
           纹理预加载                              每帧：逻辑+渲染+输入
```

**addToBatch() — 将组件送入渲染：**

```cpp
// 可变参数模板，接受任意数量的可批处理对象
void update() override {
    addToBatch(circle, rect, text, button, line);
    // 展开为: circle.writeToBatch(batch), rect.writeToBatch(batch), ...
}
```

**获取窗口控制器：**

```cpp
WindowController &wc = getWindowController();
// 可用于切换页面等操作
```

### 4.4 Context — 上下文管理器 {#context}

`Context` 提供类型安全的依赖注入，任意对象通过类型存取。

@ref context "→ Context 参考文档"

```cpp
// 存储引用（不持有所有权）
App &app = ...;
context.emplace<App>(app);

// 创建新对象（持有所有权）
context.emplace<PlayerData>("Alice", 100);

// 获取对象
auto *data = context.get<PlayerData>();
if (data) { data->score += 10; }

// 检查是否存在
if (context.has<PlayerData>()) { /* ... */ }
```

典型用法：在 App 创建后将自身注入 Context，页面中通过 `getContext()` 获取。

### 4.5 FrameLimiter — 帧率控制 {#framelimiter}

`FrameLimiter` 在 App 内部自动使用，确保稳定的目标帧率。采用"睡眠+自旋"混合等待策略，先 sleep 大部分时间，最后 2ms 自旋等待以达到精确的帧间隔。

```cpp
// App 内部
FrameLimiter limiter{60};  // 60 FPS
// 每帧结束时调用
limiter.wait();            // 阻塞至下一帧
limiter.setFPS(120);       // 动态修改帧率
```

---

## 5. 几何图形 {#graphs}

DaoLib 提供 9 种内置几何图形组件，全部实现 `dao::ifc::IGraph` 接口。

@ref graphs "→ 图形组件参考文档"

### 5.1 IGraph 接口

```cpp
class IGraph : public IDrawable {
    virtual void translate(f32 dx, f32 dy) = 0;           // 平移
    virtual void rotate(f32 cx, f32 cy, f32 theta) = 0;   // 旋转
    void writeToBatch(BatchRenderer &) const override = 0; // 渲染
};
```

### 5.2 图形速览

| 图形 | 构造参数 | 说明 |
|------|---------|------|
| [Circle](#graph-circle) | `(cx, cy, radius, color, segments)` | 圆形，triangle-fan 渲染 |
| [Ellipse](#graph-ellipse) | `(cx, cy, rx, ry, color, segments)` | 椭圆 |
| [Rectangle](#graph-rectangle) | `(x, y, w, h, color)` | 矩形，2 三角形 |
| [Triangle](#graph-triangle) | `(x1,y1, x2,y2, x3,y3, color)` | 三角形 |
| [Line](#graph-line) | `(x1,y1, x2,y2, thickness, color)` | 线段，垂直扩展成四边形 |
| [Arc](#graph-arc) | `(cx,cy, r, startA, endA, thickness, color, seg)` | 圆弧 |
| [Sector](#graph-sector) | `(cx,cy, r, startA, endA, color, seg)` | 扇形 |
| [Ring](#graph-ring) | `(cx,cy, innerR, outerR, color, seg)` | 圆环 |
| [RoundedRectangle](#graph-roundedrect) | `(x,y, w,h, cornerR, color, cornerSeg)` | 圆角矩形 |
| [Polygon\<N\>](#graph-polygon) | `(Vertex... args)` (模板) | N 边形（编译期定点数） |

### 5.3 Circle 示例 {#graph-circle}

```cpp
dao::Circle circle{400, 300, 100, dao::hexToRGBA("#e74c3c"), 64};
circle.setRadius(80);          // 修改半径（自动重建顶点）
circle.setPosition(200, 200);  // 移动圆心
circle.setColor(dao::Blue);    // 修改颜色
circle.translate(50, 0);       // 平移
circle.rotate(400, 300, 0.5f); // 绕 (400,300) 旋转 0.5 弧度
```

### 5.4 其他图形示例

```cpp
// 椭圆
dao::Ellipse ellipse{400, 300, 120, 80, dao::hexToRGBA("#3498db")};
ellipse.setRadius(150, 60);

// 矩形
dao::Rectangle rect{100, 100, 200, 150, dao::hexToRGBA("#2ecc71")};
rect.setSize(300, 200);

// 线段
dao::Line line{100, 100, 400, 300, 3.0f, dao::hexToRGBA("#9b59b6")};

// 圆弧（从 0 到 π 的半圆弧）
dao::Arc arc{400, 300, 100, 0, 3.14159f, 4.0f, dao::hexToRGBA("#e67e22")};

// 扇形（90° 扇形）
dao::Sector sector{400, 300, 100, 0, 1.5708f, dao::hexToRGBA("#1abc9c")};

// 圆环
dao::Ring ring{400, 300, 60, 100, dao::hexToRGBA("#f39c12")};

// 圆角矩形
dao::RoundedRectangle rr{100, 100, 200, 150, 20, dao::hexToRGBA("#34495e")};
rr.setCornerRadius(30);

// 五边形（模板，编译期定点数）
dao::Polygon<5> pentagon{
    dao::Vertex{400, 200, dao::Red},
    dao::Vertex{500, 270, dao::Red},
    dao::Vertex{460, 370, dao::Red},
    dao::Vertex{340, 370, dao::Red},
    dao::Vertex{300, 270, dao::Red}
};
```

### 5.5 在页面中使用图形

```cpp
class GraphDemoPage : public dao::ifc::IGeneralPage {
    dao::Circle m_circle{400, 300, 80, dao::hexToRGBA("#e74c3c")};
    dao::Rectangle m_rect{100, 100, 200, 150, dao::hexToRGBA("#3498db")};
    dao::f32 m_time = 0;

public:
    GraphDemoPage() : IGeneralPage("graph_demo") {}

    std::vector<dao::i32> getRegisterTextures() const override { return {}; }
    void open() override {}
    void close() override {}

    void update() override {
        m_time += 0.016f;
        // 动态修改圆形半径：呼吸效果
        m_circle.setRadius(80 + std::sin(m_time * 2) * 20);
        // 矩形旋转
        m_rect.rotate(200, 175, m_time * 0.5f);

        addToBatch(m_circle, m_rect);
    }

    void handleInputEvent(const SDL_Event &event) override {}
};
```

---

## 6. 文本渲染 {#text}

### 6.1 Text — 简单文本 {#text-component}

`dao::Text` 在指定位置渲染 UTF-32 文本，不处理自动换行和区域裁剪。

@ref text-component "→ Text 参考文档"

```cpp
dao::Text text{100, 100, 24,                    // x, y, 行高
               dao::hexToRGBA("#333333"),        // 颜色
               U"你好，世界！"};                  // UTF-32 文本

text.setContent(U"新内容");
text.setPosition(200, 200);
text.setLineHeight(32);
text.setColor(dao::hexToRGBA("#e74c3c"));
```

**设置字体：**

```cpp
// 在创建任何窗口之前设置
dao::GlyphAtlas::setTtfPath("./assets/ttf/zh-cn.ttf");
```

### 6.2 TextBox — 文本框 {#textbox-component}

`TextBox` 在指定矩形区域内渲染文本，支持自动换行和区域裁剪。

@ref textbox-component "→ TextBox 参考文档"

```cpp
dao::TextBox box{50, 50, 500, 300,    // x, y, width, height
                 24,                    // 行高
                 dao::White};           // 颜色

box.setContent(U"这是一段很长的文本，超出宽度将自动换行...");
box.setWordWrap(true);          // 开启自动换行（默认）
box.setTextAlignToEnd(true);    // 末尾优先显示（聊天记录风格）
box.setPosition(100, 100);
box.setSize(400, 200);
```

---

## 7. 按钮与输入 {#controls}

### 7.1 Button {#button}

@ref button "→ Button 参考文档"

```cpp
dao::Button btn{100, 100, 200, 50, [] {
    std::cout << "按钮被点击！" << std::endl;
}};

// 状态管理
btn.setStatus(dao::ButtonStatus::Disabled);  // 禁用按钮
if (btn.isEnable()) { /* ... */ }
auto status = btn.getStatus();
```

**按钮状态：** `Normal` → `Hover` → `Pressed` → `Normal`

**在页面中处理事件：**

```cpp
void handleInputEvent(const SDL_Event &event) override {
    m_button.handleEvent(event);
}
```

### 7.2 自定义按钮样式

实现 `dao::ifc::IButton` 接口可创建自定义按钮。参考 `libCode/include/components/controls/IButton.hpp`。

---

## 8. 颜色系统 {#colors}

@ref colors "→ 颜色系统参考文档"

```cpp
// 预定义颜色常量
dao::ColorRGBA c1 = dao::Red;
dao::ColorRGBA c2 = dao::Blue;
dao::ColorRGBA c3 = dao::Transparent;

// 十六进制构造
dao::ColorRGBA c4{"#e74c3c"};
dao::ColorRGBA c5{"#3498db80"};  // 带 Alpha

// 等效写法
auto c6 = dao::hexToRGBA("#2ecc71");

// 分量操作
c1.setR(128);
c1.setA(0.5f);
int r = c1.r();
float a = c1.a();
```

---

## 9. HTTP 客户端 {#http}

DaoLib 提供完整的 HTTP/HTTPS 客户端，支持同步和异步请求，内部线程池管理。

@ref http "→ HTTP 模块参考文档"

### 9.1 同步请求

```cpp
#include <web/http/HttpClient.hpp>
using namespace dao::web;

// 从 URL 构造（自动识别 HTTP/HTTPS）
auto client = HttpClient::fromURL("https://api.example.com");

// GET 请求（阻塞当前线程）
HttpResponse resp = client.get("/users");
if (resp.success) {
    std::cout << "状态: " << resp.status << "\n";
    std::cout << "响应: " << resp.body << "\n";
} else {
    std::cout << "错误: " << resp.error << "\n";
}
```

### 9.2 异步请求（回调式）

```cpp
// 异步 GET — 回调在线程池中执行
client.get("/api/data", [this](HttpResponse resp) {
    if (resp.success) {
        // 注意：此回调在线程池线程，不能直接操作 UI
        m_receivedData = std::move(resp.body);
    }
});
```

### 9.3 主线程回调投递

异步回调默认在线程池线程执行，不能操作 SDL UI。通过 `setMainThreadCallbackHandler` 将回调投递到主线程：

```cpp
// 设置主线程投递器（通常在页面 open() 中）
client.setMainThreadCallbackHandler([this](std::function<void()> task) {
    std::lock_guard lock(m_taskMutex);
    m_pendingTasks.push_back(std::move(task));
});

// 发起异步请求
client.get("/api/status", [](HttpResponse resp) {
    // 此回调会在主线程的 update() 中执行
});

// 在 update() 中执行投递的任务
void update() override {
    std::vector<std::function<void()>> tasks;
    {
        std::lock_guard lock(m_taskMutex);
        tasks.swap(m_pendingTasks);
    }
    for (auto &task : tasks) task();
    // ... 渲染逻辑
}
```

### 9.4 HttpPoller — 定时轮询 {#http-poller}

`HttpPoller` 封装定时轮询逻辑，在 `update()` 中每帧调用 `tick()`：

```cpp
#include <web/http/HttpPoller.hpp>

class MonitorPage : public dao::ifc::IGeneralPage {
    dao::web::HttpClient m_client{"192.168.1.1", 50000};
    dao::web::HttpPoller m_poller{
        m_client,
        dao::web::HttpRequest{.path = "/api/status"},
        std::chrono::seconds(5)
    };

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

### 9.5 HTTP 请求配置

```cpp
// 自定义请求头
Headers headers = {
    {"Authorization", "Bearer token123"},
    {"X-Custom", "value"}
};

// POST 请求（异步）
client.post("/api/submit", headers, R"({"key":"value"})",
    [](HttpResponse resp) { /* ... */ },
    "application/json");

// 使用 HttpRequest 发送通用请求
HttpRequest req{
    .method = HttpMethod::Post,
    .path = "/api/data",
    .headers = headers,
    .body = R"({"name":"test"})",
    .content_type = "application/json",
    .connect_timeout_sec = 5,
    .read_timeout_sec = 30
};
client.request(req, [](HttpResponse resp) { /* ... */ });
```

### 9.6 SSL 配置

```cpp
auto client = HttpClient::https("api.example.com", 443);
client.setCaCertPath("/path/to/ca-bundle.crt");
client.enableSSLVerification(true);
```

---

## 10. SQLite 数据库 {#database}

DaoLib 封装了 SQLiteCpp，提供类型安全、自动列名映射的查询接口。

@ref database "→ 数据库模块参考文档"

### 10.1 基本操作

```cpp
#include <database/sqlite/Sqlite.hpp>
using namespace dao::db;

// 打开/创建数据库
Sqlite db{"app.db", true /*createIfNotExist*/, false /*readOnly*/};

if (!db) {
    // 数据库打开失败
    return;
}

// 创建表
db.exec("CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL, "
        "score INTEGER DEFAULT 0)");

// 插入数据（{} 作为参数占位符）
db.exec("INSERT INTO users (name, score) VALUES ({}, {})",
        "Alice", 100);

db.exec("INSERT INTO users (name, score) VALUES ({}, {})",
        "Bob", 200);

// 获取最后插入的 ID
i64 lastId = db.lastInsertRowId();
```

### 10.2 查询

```cpp
// SELECT 查询 — 自动映射列名
Result result = db.select("SELECT id, name, score FROM users WHERE score > {}", 50);

for (const auto &row : result) {
    i64 id = row.asInt64("id");
    std::string name = row.asString("name");
    i32 score = row.asInt32("score");
    bool hasEmail = row.has("email");  // 检查列是否存在
}
```

### 10.3 事务

```cpp
db.beginTransaction();
db.exec("UPDATE users SET score = score + 10 WHERE id = {}", 1);
db.exec("UPDATE users SET score = score - 10 WHERE id = {}", 2);
db.commit();  // 或 db.rollback();
```

---

## 11. 系统托盘 {#tray}

@ref tray "→ Tray 参考文档"

```cpp
#include <core/frame/Tray.hpp>

class AppTray : public dao::Tray {
public:
    AppTray() : Tray("icon.png", "我的应用") {}

    void createTrayMenu() override {
        button("显示主窗口", [this](SDL_TrayEntry *) {
            getContext().get<dao::App>()->showWindow("main");
        });

        button("退出", [this](SDL_TrayEntry *) {
            getContext().get<dao::App>()->exit();
        });
    }
};

// 在 main() 中创建
app.createTray<AppTray>();
```

---

## 12. 日志系统 {#logging}

@ref logging "→ 日志系统参考文档"

### 12.1 Log 类

```cpp
#include <core/tool/Log.hpp>

// 启用彩色输出
dao::Log::openStyleOutPut();

// 各级别日志
dao::Log{dao::LogLevel::TRACE}("进入函数 foo()");
dao::Log{dao::LogLevel::DEBUG}("变量值: x={}", 42);
dao::Log{dao::LogLevel::INFO}("服务器启动成功");
dao::Log{dao::LogLevel::WARN}("内存使用率: {}%", 85);
dao::Log{dao::LogLevel::ERROR}("连接失败: {}", errMsg);

// fmt 格式化
dao::Log{dao::LogLevel::INFO}.fmt("用户 {} 登录，IP: {}", username, ip);
```

### 12.2 DAO_ERROR_LOG 宏

```cpp
DAO_ERROR_LOG("纹理加载失败: " + filePath);
// 自动附加 文件名、行号、函数名
```

---

## 13. 完整示例 — 天气查询应用 {#complete-example}

下面是一个综合运用图形、HTTP、文本的完整示例。

```cpp
#include <core/frame/App.hpp>
#include <interface/IGeneralPage.hpp>
#include <web/http/HttpClient.hpp>
#include <components/Text.hpp>
#include <components/graphs/RoundedRectangle.hpp>
#include <components/controls/SimpleButton.hpp>
#include <core/tool/Log.hpp>
#include <mutex>
#include <vector>

class WeatherPage : public dao::ifc::IGeneralPage {
    // HTTP 客户端
    dao::web::HttpClient m_http{"api.open-meteo.com", 80};

    // 主线程任务队列
    std::mutex m_taskMutex;
    std::vector<std::function<void()>> m_pendingTasks;

    // UI 组件
    dao::RoundedRectangle m_bg{0, 0, 800, 600, 0, dao::hexToRGBA("#1a1a2e")};
    dao::Text m_title{300, 30, 36, dao::hexToRGBA("#e94560"), U"天气查询"};
    dao::Text m_result{50, 150, 24, dao::hexToRGBA("#eee")};
    dao::Button m_fetchBtn{300, 500, 200, 50, [this] { fetchWeather(); }};
    dao::Text m_btnLabel{320, 510, 20, dao::hexToRGBA("#333"), U"获取天气"};

    bool m_loading = false;

    void fetchWeather() {
        if (m_loading) return;
        m_loading = true;

        m_http.get("/v1/forecast?latitude=39.9&longitude=116.4&current=temperature_2m",
                   [this](dao::web::HttpResponse resp) {
            std::lock_guard lock(m_taskMutex);
            m_pendingTasks.push_back([this, resp = std::move(resp)] {
                m_loading = false;
                if (resp.success) {
                    m_result.setContent(
                        dao::utf8::utf8to32("北京当前温度: " + resp.body));
                } else {
                    m_result.setContent(U"请求失败");
                }
            });
        });
    }

public:
    WeatherPage() : IGeneralPage("weather") {
        // 注册主线程回调投递
        m_http.setMainThreadCallbackHandler([this](std::function<void()> task) {
            std::lock_guard lock(m_taskMutex);
            m_pendingTasks.push_back(std::move(task));
        });
    }

    std::vector<dao::i32> getRegisterTextures() const override { return {}; }
    void open() override { dao::Log::openStyleOutPut(); }
    void close() override {}

    void update() override {
        // 执行主线程任务
        std::vector<std::function<void()>> tasks;
        {
            std::lock_guard lock(m_taskMutex);
            tasks.swap(m_pendingTasks);
        }
        for (auto &task : tasks) task();

        addToBatch(m_bg, m_title, m_result, m_fetchBtn, m_btnLabel);
    }

    void handleInputEvent(const SDL_Event &event) override {
        m_fetchBtn.handleEvent(event);
    }
};

int main() {
    dao::GlyphAtlas::setTtfPath("./assets/ttf/zh-cn.ttf");

    dao::App app{60};
    app.createWindow(800, 600, "main",
                     dao::Window::WorkState::Foreground)
        .addPage<WeatherPage>();
    app.run();
}
```

---

## 14. 最佳实践 {#best-practices}

1. **纹理预加载**：在 `getRegisterTextures()` 中声明所有纹理 ID，框架自动加载
2. **组件重用**：将组件声明为页面成员变量，避免每帧创建/销毁
3. **主线程安全**：异步 HTTP 回调通过 `setMainThreadCallbackHandler` 投递到主线程
4. **资源清理**：在 `close()` 中释放页面持有的外部资源
5. **字体设置**：`GlyphAtlas::setTtfPath()` 必须在创建窗口前调用
6. **Context 使用**：通过 Context 共享跨页面的数据，避免全局变量

---

## 15. 常见问题 {#faq}

**Q: 窗口不显示？**
检查 `workState` 是否为 `Foreground`，以及是否调用了 `app.run()`。

**Q: 文本显示为方块？**
确认字体路径正确，且在创建窗口前调用 `GlyphAtlas::setTtfPath()`。

**Q: 按钮不响应点击？**
确认在 `handleInputEvent()` 中调用了 `button.handleEvent(event)`。

**Q: 图形修改属性后不更新？**
DaoLib 图形的 setter 都会自动调用 `updateVertices()` 重建顶点，无需手动刷新。

**Q: HTTP 请求卡住？**
检查超时设置，同步请求会阻塞当前线程，建议使用异步请求。

**Q: 如何调试 UI 布局？**
在图形周围画矩形边框辅助定位：
```cpp
dao::Rectangle debugRect{x, y, w, h, dao::hexToRGBA("#FF000066"), false};
```

---

## 16. 模块参考索引 {#ref-index}

| 模块 | 参考文档 | 说明 |
|------|---------|------|
| 应用框架 | @ref core | App / Window / Context / FrameLimiter / Tray |
| 几何图形 | @ref graphs | Circle / Ellipse / Rectangle / Triangle / Line / Arc / Sector / Ring / RoundedRectangle / Polygon |
| UI 组件 | @ref components | Text / TextBox / Button |
| 渲染系统 | @ref render | BatchRenderer / ColorRGBA / Vertex |
| HTTP | @ref http | HttpClient / HttpPoller / HttpTypes / Headers |
| 数据库 | @ref database | Sqlite / Row / Result |
| 工具 | @ref tools | Log / BoundingBox / 类型别名 |

---

*本教程基于 DaoLib，最后更新于 2026年6月。*