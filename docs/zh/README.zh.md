# DaoLib

[中文](README.zh.md) | [English](../en/README.en.md)

一个跨平台的 C++ 应用开发框架，用于开发桌面应用、工具软件及 2D 可视化程序。

---

## 特性

- **图形渲染** - 基于 SDL3 的硬件加速渲染，采用顶点批处理技术
- **窗口管理** - 多窗口支持，前台/后台/关闭状态切换
- **几何图形** - 内置常用 2D 图形组件
- **HTTP 客户端** - 同步/异步 HTTP/HTTPS 请求
- **JSON 支持** - nlohmann/json 集成

---

## 依赖

| 库 | 版本 | 用途          |
|---|---|-------------|
| SDL3 | >= 3.4.0 | 窗口与渲染       |
| SDL3_image | >= 3.4.0 | 图像加载        |
| SDL3_ttf | >= 3.2.2 | 字体渲染        |
| cpp-httplib | >= 0.32.0 | HTTP 客户端    |
| nlohmann-json | >= 3.12.0 | JSON 处理     |
| OpenSSL | >= 3.6.1 | HTTPganS 支持 |
| utfcpp | >= 4.0.6 | UTF-8 处理    |

---

## 项目结构

```
code/
├── include/
│   ├── core/              # 核心模块
│   │   ├── frame/         # 应用框架（App, Window, Tray, Context）
│   │   ├── render/        # 渲染器（BatchRenderer, GlyphAtlas）
│   │   └── tool/          # 工具类（Point, BoundingBox, Log 等）
│   ├── components/        # UI 组件
│   │   ├── graphs/        # 几何图形（Rectangle, Circle, Line 等）
│   │   ├── Image.hpp      # 图像组件
│   │   └── Text.hpp       # 文本组件
│   ├── interface/         # 接口定义（IPage, IButton 等）
│   └── web/               # 网络模块
│       └── http/          # HTTP 客户端
└── src/                   # 实现文件
```

---

## 快速开始

### 环境要求

- C++23 编译器
- CMake >= 3.31
- vcpkg 包管理器

### 构建

```bash
# 安装依赖
vcpkg install

# 配置项目
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[vcpkg路径]/scripts/buildsystems/vcpkg.cmake

# 构建
cmake --build build
```

### 基础示例

```cpp
#include <daolib.hpp>

class MyPage : public dao::ifc::IGeneralPage {
    dao::Circle circle{100, 100, 40};

public:
    MyPage() : IGeneralPage("MyPage") {
    };

    ~MyPage() override = default;

    [[nodiscard]] std::vector<dao::i32> getRegisterTextures() const override {
        return {};
    }

    void open() override {
    }

    void close() override {
    }

    void update() override {
        addToBatch(circle);
    }

    void handleInputEvent(const SDL_Event &event) override {
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

## 模块说明

### 核心模块 (core)

#### 应用框架 (frame)

| 类 | 说明 |
|---|---|
| `App` | 应用管理器，管理窗口、托盘、主循环 |
| `Window` | 窗口类，支持前台/后台/关闭状态 |
| `Tray` | 系统托盘 |
| `Context` | 全局上下文，用于跨组件共享数据 |

#### 渲染 (render)

| 类 | 说明 |
|---|---|
| `BatchRenderer` | 批处理渲染器，支持纹理和字体渲染 |
| `GlyphAtlas` | 字形图集，管理字体纹理 |

### 组件模块 (components)

#### 几何图形 (graphs)

| 组件 | 说明 |
|---|---|
| `Rectangle` | 矩形 |
| `Circle` | 圆形 |
| `Ellipse` | 椭圆 |
| `Line` | 线段 |
| `Arc` | 弧 |
| `Sector` | 扇形 |
| `Ring` | 圆环 |
| `RoundedRectangle` | 圆角矩形 |
| `Triangle` | 三角形 |
| `Polygon` | 多边形 |

### 网络模块 (web)

#### HTTP 客户端

```cpp
#include <web/http/HttpClient.hpp>
#include <web/http/HttpsClient.hpp>

// HTTP 请求
dao::web::HttpClient http("example.com", 80);
auto resp = http.get("/api/data");

// HTTPS 请求
dao::web::HttpsClient https("api.example.com", 443);
dao::web::Headers headers = {
    {"Content-Type", "application/json"},
    {"Authorization", "Bearer token"}
};
auto resp = https.post("/api", headers, R"({"key":"value"})");

// 异步请求
auto id = https.getAsync("/api/data");
if (https.isReady(id)) {
    auto result = https.getResponse(id);
}
```

---

## 参与开发

欢迎提交 Issue 和 Pull Request 参与项目开发。

---

## 许可证

MIT License