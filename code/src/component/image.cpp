//
// Created by donghao on 2026/2/14.
//
#include <component/image.hpp>

dao::Image::Image(const float32 x, const float32 y, const float32 width, const float32 height,
    const TextureID textureID): m_texture(textureID,x, y, x + width, y + height) {
}

void dao::Image::setPosition(const float32 x, const float32 y) {
    m_texture.setPosition(x, y);
}

void dao::Image::setSize(const float32 x, const float32 y) {
    m_texture.setSize(x, y);
}

void dao::Image::writeToBatch(VertexBatchBuilder &builder) const {
    builder.addToBatch(m_texture);
}
