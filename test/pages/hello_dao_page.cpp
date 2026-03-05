#include "hello_dao_page.hpp"

using namespace dao;

std::vector<TextureID> HelloDaoPage::getRegisterTexture() const {
    return {texture::image};
}

void HelloDaoPage::open() {
}

void HelloDaoPage::close() {
}

void HelloDaoPage::update() {
    f32 len = 0;
    for (const utf32char ch: dao::utf32str(U"abcABCga")) {
        len += getGlyphAspectRatio(ch)*200;
    }

    rectangle.rotate(500,500,0.01);

    Image image1{len, 50, 150, 150, texture::image};
    addToBatch(image, text, rectangle);
}

void HelloDaoPage::handleInputEvent(const SDL_Event &event) {
    simpleButton.handleEvent(event);
}
