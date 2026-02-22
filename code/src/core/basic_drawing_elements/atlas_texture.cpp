//
// Created by donghao on 25-12-6.
//
#include <core/basic_drawing_elements/atlas_texture.hpp>

namespace dao {
    AtlasTexture::AtlasTexture(const u32 textureId,
                               const f32 left, const f32 up, const f32 right, const f32 down)
        : m_textureId(textureId), m_boundingBox(left, up, right, down) {
    }

    AtlasTexture::AtlasTexture(const u32 textureId, const BoundingBox boundingBox)
        : m_textureId(textureId), m_boundingBox(boundingBox) {
    }

    void AtlasTexture::setPosition(const f32 x, const f32 y) {
        m_boundingBox.set(
            x, y,
            x + m_boundingBox.getWidth(), y + m_boundingBox.getHeight()
        );
    }

    void AtlasTexture::setSize(const f32 width, const f32 height) {
        m_boundingBox.set(
            m_boundingBox.getLeft(), m_boundingBox.getTop(),
            m_boundingBox.getLeft()+width, m_boundingBox.getTop()+height
        );
    }
}
