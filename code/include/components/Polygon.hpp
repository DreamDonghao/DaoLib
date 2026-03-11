#pragma once
#include <core/tool/type.hpp>
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/Vertex.hpp>

namespace dao {
    template<i32 VertexNum>
    class Polygon {
    public:
        template<std::same_as<Vertex>... Args>
        explicit Polygon(Args &&... args)
            : m_vertices{std::forward<Args>(args)...} {
            static_assert(VertexNum > 2, "多边形必须至少有 3 个顶点。");
            static_assert(sizeof...(args) == VertexNum, "顶点数量不匹配");
        }

        void writeToBatch(BatchRenderer &bathRender) const {
            auto vertices = bathRender.allocateVertices(0, (m_vertices.size() - 2) * 3);
            for (i32 i = 1; i < VertexNum - 1; ++i) {
                *(vertices++) = static_cast<SDL_Vertex>(m_vertices[0]);
                *(vertices++) = static_cast<SDL_Vertex>(m_vertices[i]);
                *(vertices++) = static_cast<SDL_Vertex>(m_vertices[i + 1]);
            }
        }

    private:
        std::array<Vertex, VertexNum> m_vertices;
    };
} // dao
