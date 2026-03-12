#pragma once
#include <core/tool/type.hpp>
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/Vertex.hpp>
#include <components/graphs/IGraph.hpp>

namespace dao {
    /// @brief 多边形图形组件
    /// @details 由多个顶点定义的多边形，使用三角形扇形方式渲染
    /// @tparam VertexNum 顶点数量
    template<i32 VertexNum>
    class Polygon : public ifc::IGraph {
    public:
        template<std::same_as<Vertex>... Args>
        explicit Polygon(Args &&... args)
            : m_vertices{std::forward<Args>(args)...} {
            static_assert(VertexNum > 2, "多边形必须至少有 3 个顶点。");
            static_assert(sizeof...(args) == VertexNum, "顶点数量不匹配");
        }

        void translate(const f32 dx, const f32 dy) override {
            for (auto &vertex: m_vertices) {
                vertex.translate(dx, dy);
            }
        }

        void rotate(const f32 cx, const f32 cy, const f32 theta) override {
            for (auto &vertex: m_vertices) {
                vertex.rotate(cx, cy, theta);
            }
        }

        void writeToBatch(BatchRenderer &batchRenderer) const override {
            auto vertices = batchRenderer.allocateVertices(0, (m_vertices.size() - 2) * 3);
            for (i32 i = 1; i < VertexNum - 1; ++i) {
                *(vertices++) = m_vertices[0].getSDLVertex();
                *(vertices++) = m_vertices[i].getSDLVertex();
                *(vertices++) = m_vertices[i + 1].getSDLVertex();
            }
        }

    private:
        std::array<Vertex, VertexNum> m_vertices; ///< 多边形顶点
    };
}
