#pragma once
#include <component/image.hpp>
#include <daoui.hpp>
#include <texture_id.hpp>
#include <utility>

struct Data {
    int val = 0;
};

class TextBox : public dao::ifc::IDrawable {
public:
    TextBox(const dao::f32 x, const dao::f32 y, const dao::f32 fontSize, const dao::ColorRGBA color = dao::White,
            dao::utf32str content = U"") : text(x, y, fontSize, color, std::move(content)) {}

    void writeToBatch(dao::BatchRenderer &builder) const override { builder.addToBatch(text); }

private:
    dao::Text text;
};

class HelloDaoPage : public dao::ifc::IGeneralPage {
    TextBox text{0, 200, 200, dao::ColorRGBA("#D249FC"), U"abcA你好.。"};
    dao::Image image{10, 50, 200, 200, texture::image};
    dao::AtlasTexture img{texture::image, 10, 50, 160, 200};
    dao::SimpleButton simpleButton{10, 50, 150, 150,
                                   [this] { std::cout << getContext().get<Data>()->val << std::endl; }};
    dao::SolidRect rectangle{200, 200, 100, 100, dao::ColorRGBA("#06C763")};

public:
    HelloDaoPage() : IGeneralPage("helloDaoPage") {};

    ~HelloDaoPage() override = default;

    [[nodiscard]] std::vector<dao::TextureID> getRegisterTexture() const override;

    void open() override;

    void close() override;

    void update() override;

    void handleInputEvent(const SDL_Event &event) override;
};
