#include "hello_dao_page.hpp"

using namespace dao;

std::vector<TextureID> HelloDaoPage::getRegisterTexture() const {
    return {texture::image};
}

void HelloDaoPage::open() {
}

void HelloDaoPage::close() {
}

Polygon<4> polygon(
    Vertex(0, 0, Red),
    Vertex(100, 0, Red),
    Vertex(100, 100, Red),
    Vertex(0, 100, Red)
);

Image texture1{0, 0, 100, 100, texture::image};

void HelloDaoPage::update() {
    clearBatch();
    addToBatch(polygon,texture1);
    for (int i = 0;i < 10000;++i) {
        addToBatch(polygon,texture1);
    }
}


void HelloDaoPage::handleInputEvent(const SDL_Event &event) {

}
