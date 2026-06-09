#pragma once
#include <interface/IGeneralPage.hpp>

#include "components/TextBox.hpp"
#include "components/controls/InputBox.hpp"
#include "components/controls/SimpleButton.hpp"
#include "web/http/HttpsClient.hpp"

class HelloDaoPage : public dao::ifc::IGeneralPage {
    dao::web::HttpsClient client{"api.deepseek.com", 443};
    dao::InputBox inputBox{200, 200, 500, 100};
    dao::TextBox textbox{0, 0, 500, 140, 50,dao::Blue};
    dao::i32 reqId{-1};
    dao::Button button{
        500, 400, 200, 200, [this] {
            const dao::web::Headers headers = {
                {"Content-Type", "application/json"},
                {"Authorization", "Bearer sk-5566b22b1cfd4bc2bdc55f88c29ff5ba"}// 已删除
            };

            const dao::json body = {
                {"model", "deepseek-chat"},
                {
                    "messages", dao::json::array({
                        {{"role", "system"}, {"content", "You are a helpful assistant."}},
                        {{"role", "user"}, {"content", dao::utf32ToUtf8(inputBox.getText())}}
                    })
                },
                {"stream", false}
            };

            reqId = client.postAsync("/chat/completions", headers, body.dump());
        }
    };

public:
    HelloDaoPage() : IGeneralPage("helloDaoPage") {
    };

    ~HelloDaoPage() override = default;

    [[nodiscard]] std::vector<dao::i32> getRegisterTextures() const override;

    void open() override;

    void close() override;

    void update() override;

    void handleInputEvent(const SDL_Event &event) override;
};
