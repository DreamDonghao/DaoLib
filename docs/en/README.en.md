# DaoLib

[中文](../zh/README.zh.md) | [English](README.en.md)

A cross-platform C++ application development framework for building desktop applications, utility software, and 2D visualization programs.

---

## Features

- **Graphics Rendering** - Hardware-accelerated rendering based on SDL3 with vertex batching
- **Window Management** - Multi-window support with foreground/background/closed state switching
- **Geometric Shapes** - Built-in common 2D shape components
- **HTTP Client** - Synchronous/asynchronous HTTP/HTTPS requests
- **JSON Support** - nlohmann/json integration

---

## Dependencies

| Library | Version | Purpose |
|---|---|---|
| SDL3 | >= 3.4.0 | Window & Rendering |
| SDL3_image | >= 3.4.0 | Image Loading |
| SDL3_ttf | >= 3.2.2 | Font Rendering |
| cpp-httplib | >= 0.32.0 | HTTP Client |
| nlohmann-json | >= 3.12.0 | JSON Processing |
| OpenSSL | >= 3.6.1 | HTTPS Support |
| utfcpp | >= 4.0.6 | UTF-8 Processing |

---

## Project Structure

```
code/
├── include/
│   ├── core/              # Core Module
│   │   ├── frame/         # Application Framework (App, Window, Tray, Context)
│   │   ├── render/        # Renderers (BatchRenderer, GlyphAtlas)
│   │   └── tool/          # Utilities (Point, BoundingBox, Log, etc.)
│   ├── components/        # UI Components
│   │   ├── graphs/        # Shapes (Rectangle, Circle, Line, etc.)
│   │   ├── Image.hpp      # Image Component
│   │   └── Text.hpp       # Text Component
│   ├── interface/         # Interfaces (IPage, IButton, etc.)
│   └── web/               # Network Module
│       └── http/          # HTTP Client
└── src/                   # Implementation Files
```

---

## Quick Start

### Requirements

- C++23 Compiler
- CMake >= 3.31
- vcpkg Package Manager

### Build

```bash
# Install dependencies
vcpkg install

# Configure project
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[vcpkg path]/scripts/buildsystems/vcpkg.cmake

# Build
cmake --build build
```

### Basic Example

```cpp
#include <daolib.hpp>

class MyPage : public dao::ifc::IGeneralPage {
public:
    void init() override {
        // Add shape component
        addComponent<dao::comp::Rectangle>(100, 100, 200, 150);
    }

    void update() override {
        // Frame update logic
    }
};

int main() {
    dao::App app{60};  // 60 FPS

    app.createWindow(800, 600, "MyApp",
                     dao::Window::WorkState::Foreground,
                     true)  // Main window
        .addPage(std::make_unique<MyPage>());

    app.run();
    return 0;
}
```

---

## Module Description

### Core Module (core)

#### Application Framework (frame)

| Class | Description |
|---|---|
| `App` | Application manager for windows, tray, main loop |
| `Window` | Window with foreground/background/closed states |
| `Tray` | System tray |
| `Context` | Global context for cross-component data sharing |

#### Rendering (render)

| Class | Description |
|---|---|
| `BatchRenderer` | Batch renderer for textures and fonts |
| `GlyphAtlas` | Glyph atlas for font texture management |

### Components Module (components)

#### Geometric Shapes (graphs)

| Component | Description |
|---|---|
| `Rectangle` | Rectangle |
| `Circle` | Circle |
| `Ellipse` | Ellipse |
| `Line` | Line |
| `Arc` | Arc |
| `Sector` | Sector |
| `Ring` | Ring |
| `RoundedRectangle` | Rounded Rectangle |
| `Triangle` | Triangle |
| `Polygon` | Polygon |

### Network Module (web)

#### HTTP Client

```cpp
#include <web/http/HttpClient.hpp>
#include <web/http/HttpsClient.hpp>

// HTTP Request
dao::web::HttpClient http("example.com", 80);
auto resp = http.get("/api/data");

// HTTPS Request
dao::web::HttpsClient https("api.example.com", 443);
dao::web::Headers headers = {
    {"Content-Type", "application/json"},
    {"Authorization", "Bearer token"}
};
auto resp = https.post("/api", headers, R"({"key":"value"})");

// Async Request
auto id = https.getAsync("/api/data");
if (https.isReady(id)) {
    auto result = https.getResponse(id);
}
```

---

## Contributing

Issues and Pull Requests are welcome.

---

## License

MIT License