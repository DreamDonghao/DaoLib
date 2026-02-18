#include <core/frame/app.hpp>
#include <hello_dao_page/hello_dao_page.hpp>

int main(int argc, char *argv[]) {
    using namespace example;
    /// 创建一个应用
    dao::App app(60);

    /// 创建一个窗口并添加一个页面
    app.createWindow(800, 600, "设置", false, false,
                     true, false, false, false)
            .addPage(std::make_unique<example::HelloDaoPage>());

    app.run();
    return 0;
}
