//
// Created by donghao on 2026/2/14.
//
#ifndef TEST_IMAGE_HPP
#define TEST_IMAGE_HPP
#include <core/basic_drawing_elements/atlas_texture.hpp>
#include <interface/drawable.hpp>

namespace dao {
    class Image : public Drawable {
    public:
        Image(float32 x, float32 y,float32 width, float32 height, TextureID textureID);

        void setPosition(float32 x, float32 y);

        void setSize(float32 x, float32 y);

        void writeToBatch(VertexBatchBuilder &builder) const override;

    private:
        AtlasTexture m_texture;
    };
} // dao

#endif //TEST_IMAGE_HPP
