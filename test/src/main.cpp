//
// Created by donghao on 25-11-28.
//
#include <test_page.hpp>
#include <test_page_1.hpp>
#include "core/frame/app.hpp"
int main(int argc, char *argv[]) {
    /// 创建一个应用
    auto &app = dao::App::getApp();

    /// 创建一个窗口并添加一个页面
    app.createWindow(1600, 1000,true,true,true,false)
            .addPage(std::make_unique<TestPage>())
            .addPage(std::make_unique<TestPage1>());
    /// 启动程序
    app.run();
    return 0;
}
