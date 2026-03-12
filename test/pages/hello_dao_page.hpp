#pragma once
#include <interface/IGeneralPage.hpp>

class HelloDaoPage : public dao::ifc::IGeneralPage {

public:
    HelloDaoPage() : dao::ifc::IGeneralPage("helloDaoPage") {};

    ~HelloDaoPage() override = default;

    [[nodiscard]] std::vector<dao::i32> getRegisterTextures() const override;

    void open() override;

    void close() override;

    void update() override;

    void handleInputEvent(const SDL_Event &event) override;
};
