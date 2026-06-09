#include <core/frame/App.hpp>
#include "tray/AppTray.hpp"
#include <database/sqlite/Sqlite.hpp>
#include "pages/SettingsPage/SettingPage.hpp"
#include "pet/Pet.hpp"

int main(int argc, char *argv[]) {
    dao::Log::openStyleOutPut();
    //
    // dao::Log{dao::LogLevel::TRACE}("最细粒度的调试信息，记录程序的每一步执行");
    // dao::Log{dao::LogLevel::DEBUG}("调试信息，帮助理解程序运行状态");
    // dao::Log{dao::LogLevel::INFO}("常规信息，记录程序正常运行的重要事件");
    // dao::Log{dao::LogLevel::WARN}("警告信息，表示可能出现问题但不影响程序继续运行");
    // dao::Log{dao::LogLevel::ERROR}("错误信息，表示程序遇到了问题，但可以继续运行");
    // dao::Log{dao::LogLevel::FATAL}("致命错误，程序无法继续运行，即将退出");
    //
    // dao::db::Sqlite db("./testelsdi.db3", true, false);
    //
    // db.exec(R"sql(INSERT INTO testdb(name,age VALUES("李四",{}))sql",100);
    // for (auto result = db.select(R"sql(SELECT age, name FROM testdb;)sql");
    //      auto item: result) {
    //     std::cout << item.asString("name") << std::endl;
    //     std::cout << item.asInt64("age") << std::endl;
    // }
    //
    // return 0;
    dao::App app{120, true};

    app.getContext().emplace<dao::App>(app);

    // app.createWindow(1000, 1000, "2", dao::Window::WorkState::Foreground, true, false)
    //         .addPage<test::PageA>();
    // /// 创建窗口
    // app.createWindow(1300, 1300, "设置", dao::Window::WorkState::Foreground, true, true, true, false)
    //         .addPage<HelloDaoPage>();

    app.createWindow(100, 130, "pet", dao::Window::WorkState::Foreground,
                     false, false, true, true, true)
            .addPage<test::Pet>();

    app.createWindow(600, 400, "SettingsPage", dao::Window::WorkState::Closed,
                     false, true, false, false, false)
            .addPage<test::SettingsPage>();

    /// 创建托盘
    app.createTray<AppTray>("./assets/textures/atlas/icon.png", "托盘").createTrayMenu();

    /// 运行应用
    app.run();

    return 0;
}
