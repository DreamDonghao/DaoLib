//
// Created by donghao on 25-12-8.
//
#include "test_page.hpp"
#include <component/rectangle.hpp>

std::vector<uint32_t> TestPage::getRegisterTexture() const {
    std::vector registerTexture{
        texture::food_apple,
        texture::food_banana,
    };
    return registerTexture;
}

void TestPage::update() {
    m_vertexBatch.clearDrawBatches();
    for (int i = 100; i; --i) {
        m_vertexBatch.addToBatch(appleImg);
        m_vertexBatch.addToBatch(bananaImg);
        rectangle2.writeToBatch(m_vertexBatch);
        rectangle3.writeToBatch(m_vertexBatch);
        buttonStyle.writeToBatch(m_vertexBatch);
    }
    for (int i = 10000; i; --i) {
        m_vertexBatch.addToBatch(appleImg);
        m_vertexBatch.addToBatch(bananaImg);
    }
}

void TestPage::handleInputEvent(const SDL_Event &event) {
    button.handleEvent(event);
}


const std::vector<dao::AtlasDrawBatch> &TestPage::getDrawBatches() const {
    return m_vertexBatch.getDrawBatches();
}
