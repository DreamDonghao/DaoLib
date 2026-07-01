@page use Develop with DaoLib

# Develop with DaoLib

## Tutorial

@ref tutorial "→ DaoLib Tutorial (Chinese)" — from setup to complete application.

## Module Reference (Chinese)

| Module | Link | Description |
|--------|------|-------------|
| Core | @ref core | App / Window / Context / FrameLimiter / Tray |
| Graphs | @ref graphs | 9 graph components, IGraph interface |
| Components | @ref components | Text / TextBox / Button |
| Render | @ref render | BatchRenderer / ColorRGBA / Vertex |
| HTTP | @ref http | HttpClient / HttpPoller / HttpTypes / Headers |
| Database | @ref database | Sqlite / Row / Result |
| Tools | @ref tools | Log / BoundingBox / Type aliases |

## Quick Start

```cpp
#include <core/frame/App.hpp>
#include <interface/IGeneralPage.hpp>
#include <components/Text.hpp>

class HelloPage : public dao::ifc::IGeneralPage {
    dao::Text m_text{100, 100, 32, dao::hexToRGBA("#333"), U"Hello DaoLib!"};
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