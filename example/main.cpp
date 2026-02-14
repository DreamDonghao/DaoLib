#include <core/frame/app.hpp>
#include "pages/hello_dao_page/hello_dao_page.hpp"

int main(int argc, char *argv[]) {
    /// 创建一个应用
    auto &app = dao::App::getApp();

    /// 创建一个窗口并添加一个页面
    app.createWindow(300, 300,true)
            .addPage(std::make_unique<HelloDaoPage>());

    app.run();
    return 0;
}
