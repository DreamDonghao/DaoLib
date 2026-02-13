#include "hello_dao_page.hpp"

std::vector<uint32> HelloDaoPage::getRegisterTexture() const {
    return {texture::image};
}

void HelloDaoPage::init() {
}

void HelloDaoPage::close() {
}

void HelloDaoPage::update() {
    m_vertexBatch.clearDrawBatches();
    m_vertexBatch.addToBatch(text);
    m_vertexBatch.addToBatch(img);
}

void HelloDaoPage::handleInputEvent(const SDL_Event &event) {
}
