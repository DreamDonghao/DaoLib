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
   std::cout << "Hello Dao Page" << std::endl;
}


void HelloDaoPage::handleInputEvent(const SDL_Event &event) {
    simpleButton.handleEvent(event);
}
