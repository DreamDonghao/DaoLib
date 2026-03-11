#include <core/render/Texture.hpp>

namespace dao {
    Texture::Texture(const TextureID textureId,
                               const f32 left, const f32 top, const f32 right, const f32 bottom)
        : m_textureId(textureId), m_boundingBox(left, top, right, bottom) {
    }

    Texture::Texture(const TextureID textureId, const BoundingBox boundingBox)
        : m_textureId(textureId), m_boundingBox(boundingBox) {
    }

    void Texture::setPosition(const f32 x, const f32 y) {
        m_boundingBox.set(
            x, y,
            x + m_boundingBox.getWidth(), y + m_boundingBox.getHeight()
        );
    }

    void Texture::setSize(const f32 width, const f32 height) {
        m_boundingBox.set(
            m_boundingBox.getLeft(), m_boundingBox.getTop(),
            m_boundingBox.getLeft()+width, m_boundingBox.getTop()+height
        );
    }

    TextureID Texture::getTextureID() const { return m_textureId; }

    const BoundingBox & Texture::getBoundingBox() const {
        return m_boundingBox;
    }
}
