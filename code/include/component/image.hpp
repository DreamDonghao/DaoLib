#pragma once
#include <core/render/atlas_texture.hpp>
#include <interface/drawable.hpp>

namespace dao {
    class Image : public Drawable {
    public:
        Image(f32 x, f32 y,f32 width, f32 height, TextureID textureID);

        void setPosition(f32 x, f32 y);

        void setSize(f32 x, f32 y);

        void writeToBatch(VertexBatchBuilder &builder) const override;

    private:
        AtlasTexture m_texture;
    };
}