#include "hello_dao_page.hpp"


using namespace dao;

std::vector<u32> HelloDaoPage::getRegisterTexture() const {
    return {texture::image};
}

void HelloDaoPage::init() {
}

void HelloDaoPage::close() {
}

void HelloDaoPage::update() {
    f32 len = 0;
    for (const utf32char ch: dao::utf32str(U"abcABC123你")) {
        len += getGlyphAspectRatio(ch)*200;
    }

    dao::Image image1{len, 50, 150, 150, texture::image};
    dao::Rectangle rectangle{len, 0, 100, 500, dao::hexToRGBA("#00D980")};
    addToBatch(image, text, rectangle);
}

void HelloDaoPage::handleInputEvent(const SDL_Event &event) {
    simpleButton.handleEvent(event);
}
