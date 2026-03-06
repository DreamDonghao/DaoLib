#include "hello_dao_page.hpp"

using namespace dao;

std::vector<TextureID> HelloDaoPage::getRegisterTexture() const {
    return {texture::image};
}

void HelloDaoPage::open() {

}

void HelloDaoPage::close() {
}

Image image1{1, 50, 150, 150, texture::image};
dao::Circle circle{500, 500, 100, dao::ColorRGBA("#185ABD"), dao::ColorRGBA("#FE2857")};

Polygon a{Vertex{0, 0,}, Vertex{100, 0}, Vertex{100, 100}};

void HelloDaoPage::update() {
    clearBatch();
    f32 len = 0;
    for (const utf32char ch: dao::utf32str(U"abcABCga")) {
        len += getGlyphAspectRatio(ch) * 200;
    }
    rectangle.rotate(500, 500, 0.01);

    for (i32 i = 0; i < 1000000; ++i) {
        addToBatch(a);
    }
}


void HelloDaoPage::handleInputEvent(const SDL_Event &event) {
    simpleButton.handleEvent(event);
}
