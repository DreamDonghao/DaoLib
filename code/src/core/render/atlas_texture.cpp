#include <core/render/atlas_texture.hpp>

namespace dao {
    AtlasTexture::AtlasTexture(const TextureID textureId,
                               const f32 left, const f32 top, const f32 right, const f32 bottom)
        : m_textureId(textureId), m_boundingBox(left, top, right, bottom) {
    }

    AtlasTexture::AtlasTexture(const TextureID textureId, const BoundingBox boundingBox)
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

    TextureID AtlasTexture::getTextureID() const { return m_textureId; }

    const BoundingBox & AtlasTexture::getBoundingBox() const {
        return m_boundingBox;
    }
}
