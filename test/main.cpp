#include <core/frame/app.hpp>
#include "hello_dao_page.hpp"
#include "tray/my_tray.hpp"

int main(int argc, char *argv[]) {
    /// 创建应用
    dao::App app{60,true};

    app.getContext().emplaceService<dao::App*>(&app);

    /// 创建窗口
    app.createWindow(1300, 1300, "设置",false, false, true, true)
            .addPage(std::make_unique<HelloDaoPage>());

    app.createWindow(300, 300, "2",true, false)
            .addPage(std::make_unique<HelloDaoPage>());
    /// 创建托盘
    app.createTray<myTray>("./assets/textures/atlas/icon.png", "托盘")
            .createTrayMenu();

    /// 运行应用
    app.run();

    return 0;
}
