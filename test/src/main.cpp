//
// Created by donghao on 25-11-28.
//
#include <test_page.hpp>
#include <test_page_1.hpp>
#include "core/frame/app.hpp"
#include <pats.hpp>
int main(int argc, char *argv[]) {
    /// 创建一个应用
    auto &app = dao::App::getApp();

    /// 创建一个窗口并添加一个页面
    app.createWindow(200, 200,true,true,true,true)
            .addPage(std::make_unique<PatsPage>())
            .addPage(std::make_unique<TestPage>())
            .addPage(std::make_unique<TestPage1>());
    /// 启动程序
    app.run();
    return 0;
}
