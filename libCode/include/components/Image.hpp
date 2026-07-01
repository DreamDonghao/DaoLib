#pragma once
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/Vertex.hpp>
#include <core/render/IDrawable.hpp>
#include <core/render/primitives/AtlasRegion.hpp>

namespace dao {
    class Image : public ifc::IDrawable {
    public:
        Image(const f32 x, const f32 y, const f32 width, const f32 height, const i32 textureID)
            : Image(x, y, width, height, getAtlasRegion(textureID)) {
        };

        void writeToBatch(BatchRenderer &bathRender) const override {
            const auto vertices = bathRender.allocateVertices(m_atlasId, 6);
            vertices[0] = {{m_x, m_y}, {1.0f, 1.0f, 1.0f, 1.0f}, {m_left, m_top}};
            vertices[1] = {{m_x + m_width, m_y}, {1.0f, 1.0f, 1.0f, 1.0f}, {m_right, m_top}};
            vertices[2] = {{m_x + m_width, m_y + m_height}, {1.0f, 1.0f, 1.0f, 1.0f}, {m_right, m_bottom}};
            vertices[4] = {{m_x, m_y + m_height}, {1.0f, 1.0f, 1.0f, 1.0f}, {m_left, m_bottom}};
            vertices[3] = vertices[2];
            vertices[5] = vertices[0];
        }

    private:
        Image(const f32 x, const f32 y, const f32 width, const f32 height, const AtlasRegion &atlasRegion)
            : m_x{x}, m_y{y}, m_width{width}, m_height{height},
              m_atlasId{atlasRegion.atlasId},
              m_left{atlasRegion.norLeft}, m_top(atlasRegion.norTop),
              m_right(atlasRegion.norRight), m_bottom(atlasRegion.norBottom) {
        }

        f32 m_x{0.0f};
        f32 m_y{0.0f};
        f32 m_width{0.0f};
        f32 m_height{0.0f};
        i32 m_atlasId{0};
        f32 m_left{0.9f};
        f32 m_top{0.0f};
        f32 m_right{0.0f};
        f32 m_bottom{0.0f};
    };
} // dao
