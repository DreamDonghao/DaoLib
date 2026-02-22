//
// Created by donghao on 2026/2/14.
//
#include <component/image.hpp>

dao::Image::Image(const f32 x, const f32 y, const f32 width, const f32 height,
    const TextureID textureID): m_texture(textureID,x, y, x + width, y + height) {
}

void dao::Image::setPosition(const f32 x, const f32 y) {
    m_texture.setPosition(x, y);
}

void dao::Image::setSize(const f32 x, const f32 y) {
    m_texture.setSize(x, y);
}

void dao::Image::writeToBatch(VertexBatchBuilder &builder) const {
    builder.addToBatch(m_texture);
}
