#pragma once
#include <daoui.hpp>
#include <texture_id.hpp>
#include <component/image.hpp>
#include <utility>

struct Data {
    int val = 0;
};

class TextBox : public dao::Drawable {
public:
    TextBox(const dao::f32 x, const dao::f32 y, const dao::f32 fontSize, const dao::ColorRGBA color = {0, 0, 0, 1},
            dao::utf32str content = U"") : text(x, y, fontSize, color, std::move(content)) {
    }

    void writeToBatch(dao::VertexBatchBuilder &builder) const override {
        builder.addToBatch(text);
    }

private:
    dao::Text text;
};

class HelloDaoPage : public dao::GeneralPage {
    TextBox text{0, 200, 200, dao::hexToRGBA("#D249FC"), U"abcABCgabcdefghizklmnopq123你好.。"};
    dao::Image image{10, 50, 200, 200, texture::image};
    dao::AtlasTexture img{texture::image, 10, 50, 160, 200};
    dao::SimpleButton simpleButton{
        10, 50, 150, 150, [this] {
            std::cout << getContext().getState<Data>().val << std::endl;
        }
    };

public:
    HelloDaoPage() : GeneralPage("helloDaoPage") {
    };

    ~HelloDaoPage() override = default;

    [[nodiscard]] std::vector<dao::u32> getRegisterTexture() const override;

    void init() override;

    void close() override;

    void update() override;

    void handleInputEvent(const SDL_Event &event) override;
};
