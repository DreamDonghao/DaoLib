#include <core/frame/App.hpp>
#include "CellularAutomataPage/CellularAutomataPage.hpp"
#include "TestStaticDraw.hpp"
dao::i32 dao::Config::defaultDynamicVerticesCount = 1000000;
std::string_view dao::Config::ttfPath = "./assets/ttf/xs.ttf";
int main(int argc, char *argv[]) {

    // 像素字体配置：glyphSize = 字体设计像素尺寸（如 12/16/24）
    // pixelFont=true → NEAREST 过滤 + 无抗锯齿渲染
    dao::GlyphAtlas::configure("./assets/ttf/xs.ttf",12, 512, true);

    dao::App app{120, false};
    app.getContext().emplace<dao::App>(app);

    app.createWindow(1200, 800, "文字与输入组件测试 — Text & Input Test",
                     dao::Window::WorkState::Foreground, true, true)
        .addPage<test::CellularAutomataPage>();

    app.run();
    return 0;
}
