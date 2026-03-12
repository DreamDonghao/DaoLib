#include <core/frame/App.hpp>
#include "hello_dao_page.hpp"
#include "page_a/page_a.hpp"
#include "tray/AppTray.hpp"

int main(int argc, char *argv[]) {
    dao::App app{120, true};

    app.getContext().emplace<dao::App>(app);

    app.createWindow(1000, 1000, "2", dao::Window::WorkState::Foreground, true, false)
            .addPage(std::make_unique<test::PageA>());
    /// 创建窗口
    app.createWindow(1300, 1300, "设置", dao::Window::WorkState::Foreground, true, true, true, false)
            .addPage(std::make_unique<HelloDaoPage>());

    /// 创建托盘
    app.createTray<AppTray>("./assets/textures/atlas/icon.png", "托盘").createTrayMenu();

    /// 运行应用
    app.run();

    return 0;
}
