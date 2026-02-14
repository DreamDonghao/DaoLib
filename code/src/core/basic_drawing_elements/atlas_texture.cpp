//
// Created by donghao on 25-12-6.
//
#include <core/basic_drawing_elements/atlas_texture.hpp>

namespace dao {
    AtlasTexture::AtlasTexture(const uint32 textureId,
                               const float32 left, const float32 up, const float32 right, const float32 down)
        : m_textureId(textureId), m_boundingBox(left, up, right, down) {
    }

    AtlasTexture::AtlasTexture(const uint32 textureId, const BoundingBox boundingBox)
        : m_textureId(textureId), m_boundingBox(boundingBox) {
    }

    void AtlasTexture::setPosition(const float32 x, const float32 y) {
        m_boundingBox.set(
            x, y,
            x + m_boundingBox.getWidth(), y + m_boundingBox.getHeight()
        );
    }

    void AtlasTexture::setSize(const float32 width, const float32 height) {
        m_boundingBox.set(
            m_boundingBox.getLeft(), m_boundingBox.getTop(),
            m_boundingBox.getLeft()+width, m_boundingBox.getTop()+height
        );
    }
}
