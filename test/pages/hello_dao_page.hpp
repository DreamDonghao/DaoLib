#pragma once
#include <interface/IGeneralPage.hpp>
#include <web/http/HttpClient.hpp>

class HelloDaoPage : public dao::ifc::IGeneralPage {
    dao::web::HttpClient client{"127.0.0.1", 8080};

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
