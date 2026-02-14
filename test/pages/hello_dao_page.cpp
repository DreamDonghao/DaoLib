#include "hello_dao_page.hpp"
using namespace dao;

std::vector<uint32> HelloDaoPage::getRegisterTexture() const {
    return {texture::image};
}

void HelloDaoPage::init() {
}

void HelloDaoPage::close() {
}

void HelloDaoPage::update() {
    addToBatch(image);
}

void HelloDaoPage::handleInputEvent(const SDL_Event &event) {
}
