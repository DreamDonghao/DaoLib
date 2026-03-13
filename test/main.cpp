#include <core/frame/App.hpp>
#include "hello_dao_page.hpp"
#include "page_a/page_a.hpp"
#include "tray/AppTray.hpp"
#include <web/http/HttpsClient.hpp>

int main(int argc, char *argv[]) {
    dao::web::HttpsClient client("api.deepseek.com", 443);

    const dao::web::Headers headers = {
        {"Content-Type", "application/json"},
        {"Authorization", "Bearer sk-b9f3ee3a3ddd45ebb31d4a918c828217"}// key已删除
    };

    const dao::json body = {
        {"model", "deepseek-chat"},
        {"messages", dao::json::array({
            {{"role", "system"}, {"content", "You are a helpful assistant."}},
            {{"role", "user"}, {"content", "你好!"}}
        })},
        {"stream", false}
    };

    if (const auto resp = client.post("/chat/completions", headers, body.dump()); resp.success) {
        std::cout << resp.body << std::endl;
    } else {
        std::cout << "Error: " << resp.error << std::endl;
    }

    return 0;

    dao::App app{120, true};

    app.getContext().emplace<dao::App>(app);

    app.createWindow(1000, 1000, "2", dao::Window::WorkState::Foreground, true, false)
            。addPage(std::make_unique<test::PageA>());
    /// 创建窗口
    app.createWindow(1300, 1300, "设置", dao::Window::WorkState::Foreground, true, true, true, false)
            。addPage(std::make_unique<HelloDaoPage>());

    /// 创建托盘
    app.createTray<AppTray>("./assets/textures/atlas/icon.png", "托盘").createTrayMenu();

    /// 运行应用
    app.run();

    return 0;
}
