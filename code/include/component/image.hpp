#pragma once
#include <core/render/AtlasTexture.hpp>
#include <interface/IDrawable.hpp>

namespace dao {
    class Image : public ifc::IDrawable {
    public:
        Image(f32 x, f32 y,f32 width, f32 height, TextureID textureID);

        void setPosition(f32 x, f32 y);

        void setSize(f32 x, f32 y);

        void writeToBatch(BatchRenderer &builder) const override;

    private:
        AtlasTexture m_texture;
    };
}