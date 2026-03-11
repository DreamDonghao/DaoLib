#pragma once
#include <component/image.hpp>
#include <daoui.hpp>
#include <texture_id.hpp>
#include <utility>

struct Data {
    int val = 0;
};


class HelloDaoPage : public dao::ifc::IGeneralPage {



public:
    HelloDaoPage() : IGeneralPage("helloDaoPage") {};

    ~HelloDaoPage() override = default;

    [[nodiscard]] std::vector<dao::TextureID> getRegisterTexture() const override;

    void open() override;

    void close() override;

    void update() override;

    void handleInputEvent(const SDL_Event &event) override;
};
