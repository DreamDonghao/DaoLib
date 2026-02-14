#pragma once
#include <daoui.hpp>
#include <texture_id.hpp>
#include <component/image.hpp>
using namespace dao;

class HelloDaoPage :public GeneralPage {
    Image image{10,50,150,150,texture::image};
public:
    HelloDaoPage() : GeneralPage("helloDaoPage") {
    };

    ~HelloDaoPage() override = default;

    [[nodiscard]] std::vector<uint32> getRegisterTexture() const override;

    void init() override;

    void close() override;

    void update() override;

    void handleInputEvent(const SDL_Event &event) override;
};
