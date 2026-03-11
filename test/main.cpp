#include <core/frame/app.hpp>
#include "hello_dao_page.hpp"
#include "page_a/page_a.hpp"
#include "tray/AppTray.hpp"

int main(int argc, char *argv[]) {
    dao::App app{-1, true};

    app.getContext().emplace<dao::App>(app);

    /// 创建窗口
    app.createWindow(1300, 1300, "设置", dao::Window::WorkState::Foreground, true, true, true, false)
            .addPage(std::make_unique<HelloDaoPage>());

    app.createWindow(300, 300, "2", dao::Window::WorkState::Background, true, false)
            .addPage(std::make_unique<test::PageA>());
    /// 创建托盘
    app.createTray<AppTray>("./assets/textures/atlas/icon.png", "托盘").createTrayMenu();

    /// 运行应用
    app.run();

    return 0;
}
