# DaoLib

[中文](docs/zh/README.zh.md) | [English](docs/en/README.en.md)

一个跨平台的 C++ 应用开发框架，用于开发桌面应用、工具软件及 2D 可视化程序。

A cross-platform C++ application development framework for building desktop applications, utility software, and 2D visualization programs.

---

## 特性 / Features

- **图形渲染 / Graphics Rendering** - 基于 SDL3 的硬件加速渲染，采用顶点批处理技术 / Hardware-accelerated rendering based on SDL3 with vertex batching
- **窗口管理 / Window Management** - 多窗口支持，前台/后台/关闭状态切换 / Multi-window support with foreground/background/closed state switching
- **几何图形 / Geometric Shapes** - 内置常用 2D 图形组件 / Built-in common 2D shape components
- **HTTP 客户端 / HTTP Client** - 同步/异步 HTTP/HTTPS 请求 / Synchronous/asynchronous HTTP/HTTPS requests
- **JSON 支持 / JSON Support** - nlohmann/json 集成 / nlohmann/json integration

---

## 依赖 / Dependencies

| 库 / Library | 版本 / Version | 用途 / Purpose |
|---|---|---|
| SDL3 | >= 3.4.0 | 窗口与渲染 / Window & Rendering |
| SDL3_image | >= 3.4.0 | 图像加载 / Image Loading |
| SDL3_ttf | >= 3.2.2 | 字体渲染 / Font Rendering |
| cpp-httplib | >= 0.32.0 | HTTP 客户端 / HTTP Client |
| nlohmann-json | >= 3.12.0 | JSON 处理 / JSON Processing |
| OpenSSL | >= 3.6.1 | HTTPS 支持 / HTTPS Support |
| utfcpp | >= 4.0.6 | UTF-8 处理 / UTF-8 Processing |

---

## 项目结构 / Project Structure

```
code/
├── include/
│   ├── core/              # 核心模块 / Core Module
│   │   ├── frame/         # 应用框架 / App Framework (App, Window, Tray, Context)
│   │   ├── render/        # 渲染器 / Renderers (BatchRenderer, GlyphAtlas)
│   │   └── tool/          # 工具类 / Utilities (Point, BoundingBox, Log, etc.)
│   ├── components/        # UI 组件 / UI Components
│   │   ├── graphs/        # 几何图形 / Shapes (Rectangle, Circle, Line, etc.)
│   │   ├── Image.hpp      # 图像组件 / Image Component
│   │   └── Text.hpp       # 文本组件 / Text Component
│   ├── interface/         # 接口定义 / Interfaces (IPage, IButton, etc.)
│   └── web/               # 网络模块 / Network Module
│       └── http/          # HTTP 客户端 / HTTP Client
└── src/                   # 实现文件 / Implementation Files
```

---

## 快速开始 / Quick Start

### 环境要求 / Requirements

- C++23 编译器 / Compiler
- CMake >= 3.31
- vcpkg 包管理器 / Package Manager

### 构建 / Build

```bash
# 安装依赖 / Install dependencies
vcpkg install

# 配置项目 / Configure project
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[vcpkg path]/scripts/buildsystems/vcpkg.cmake

# 构建 / Build
cmake --build build
```

### 基础示例 / Basic Example

```cpp
#include <daolib.hpp>

class MyPage : public dao::ifc::IGeneralPage {
public:
    void init() override {
        // 添加图形组件 / Add shape component
        addComponent<dao::comp::Rectangle>(100, 100, 200, 150);
    }

    void update() override {
        // 每帧更新逻辑 / Frame update logic
    }
};

int main() {
    dao::App app{60};  // 60 FPS

    app.createWindow(800, 600, "MyApp",
                     dao::Window::WorkState::Foreground,
                     true)  // 主体窗口 / Main window
        .addPage(std::make_unique<MyPage>());

    app.run();
    return 0;
}
```

---

## 模块说明 / Module Description

### 核心模块 (core) / Core Module

#### 应用框架 (frame) / Application Framework

| 类 / Class | 说明 / Description |
|---|---|
| `App` | 应用管理器，管理窗口、托盘、主循环 / Application manager for windows, tray, main loop |
| `Window` | 窗口类，支持前台/后台/关闭状态 / Window with foreground/background/closed states |
| `Tray` | 系统托盘 / System tray |
| `Context` | 全局上下文，用于跨组件共享数据 / Global context for cross-component data sharing |

#### 渲染 (render) / Rendering

| 类 / Class | 说明 / Description |
|---|---|
| `BatchRenderer` | 批处理渲染器，支持纹理和字体渲染 / Batch renderer for textures and fonts |
| `GlyphAtlas` | 字形图集，管理字体纹理 / Glyph atlas for font texture management |

### 组件模块 (components) / Components Module

#### 几何图形 (graphs) / Geometric Shapes

| 组件 / Component | 说明 / Description |
|---|---|
| `Rectangle` | 矩形 / Rectangle |
| `Circle` | 圆形 / Circle |
| `Ellipse` | 椭圆 / Ellipse |
| `Line` | 线段 / Line |
| `Arc` | 弧 / Arc |
| `Sector` | 扇形 / Sector |
| `Ring` | 圆环 / Ring |
| `RoundedRectangle` | 圆角矩形 / Rounded Rectangle |
| `Triangle` | 三角形 / Triangle |
| `Polygon` | 多边形 / Polygon |

### 网络模块 (web) / Network Module

#### HTTP 客户端 / HTTP Client

```cpp
#include <web/http/HttpClient.hpp>
#include <web/http/HttpsClient.hpp>

// HTTP 请求 / HTTP Request
dao::web::HttpClient http("example.com", 80);
auto resp = http.get("/api/data");

// HTTPS 请求 / HTTPS Request
dao::web::HttpsClient https("api.example.com", 443);
dao::web::Headers headers = {
    {"Content-Type", "application/json"},
    {"Authorization", "Bearer token"}
};
auto resp = https.post("/api", headers, R"({"key":"value"})");

// 异步请求 / Async Request
auto id = https.getAsync("/api/data");
if (https.isReady(id)) {
    auto result = https.getResponse(id);
}
```

---

## 参与开发 / Contributing

欢迎提交 Issue 和 Pull Request 参与项目开发。

Issues and Pull Requests are welcome.

---

## 许可证 / License

MIT License