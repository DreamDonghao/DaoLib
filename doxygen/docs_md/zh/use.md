@page use 使用 DaoLib 开发

[TOC]

# 使用 DaoLib 开发

## 入门教程

@ref tutorial "→ DaoLib 从零到精通教程" — 从环境搭建到完整应用，涵盖所有核心功能。

## 模块参考

| 模块 | 链接 | 说明 |
|------|------|------|
| 核心框架 | @ref core | App / Window / Context / FrameLimiter / Tray |
| 几何图形 | @ref graphs | 9 种图形组件，IGraph 接口 |
| UI 组件 | @ref components | Text / TextBox / Button |
| 渲染系统 | @ref render | BatchRenderer / ColorRGBA / Vertex |
| HTTP | @ref http | HttpClient / HttpPoller / HttpTypes / Headers |
| 数据库 | @ref database | Sqlite / Row / Result |
| 工具 | @ref tools | Log / BoundingBox / 类型别名 |

## 快速开始

```cpp
#include <core/frame/App.hpp>
#include <interface/IGeneralPage.hpp>
#include <components/Text.hpp>

class HelloPage : public dao::ifc::IGeneralPage {
    dao::Text m_text{100, 100, 32, dao::hexToRGBA("#333"), U"你好！"};
public:
    HelloPage() : IGeneralPage("hello") {}
    std::vector<dao::i32> getRegisterTextures() const override { return {}; }
    void open() override {}
    void close() override {}
    void update() override { addToBatch(m_text); }
    void handleInputEvent(const SDL_Event &) override {}
};

int main() {
    dao::GlyphAtlas::setTtfPath("./assets/ttf/zh-cn.ttf");
    dao::App app{60};
    app.createWindow(800, 600, "main", dao::Window::WorkState::Foreground)
        .addPage<HelloPage>();
    app.run();
}
```