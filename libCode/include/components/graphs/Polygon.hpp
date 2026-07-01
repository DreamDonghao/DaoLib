#pragma once
#include <core/tool/type.hpp>
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/Vertex.hpp>
#include <components/graphs/IGraph.hpp>

namespace dao {
    /// @brief 多边形图形组件
    /// @details 由多个顶点定义的多边形，以第一个顶点为共同顶点的三角形扇形方式渲染
    /// @tparam VertexNum 顶点数量（编译期常量，必须大于 2）
    template<i32 VertexNum>
    class Polygon : public ifc::IGraph {
    public:
        /// @brief 构造函数
        /// @tparam Args 顶点参数包，类型必须为 Vertex
        /// @param args 顶点列表，数量必须与 VertexNum 一致
        template<std::same_as<Vertex>... Args>
        explicit Polygon(Args &&... args)
            : m_vertices{std::forward < Args > (args)...} {
            static_assert(VertexNum > 2, "多边形必须至少有 3 个顶点。");
            static_assert(sizeof...(args) == VertexNum, "顶点数量不匹配");
        }

        /// @brief 平移图形
        /// @param dx x 方向平移量
        /// @param dy y 方向平移量
        void translate(const f32 dx, const f32 dy) override {
            for (auto &vertex: m_vertices) {
                vertex.translate(dx, dy);
            }
        }

        /// @brief 旋转图形
        /// @param cx 旋转中心 x 坐标
        /// @param cy 旋转中心 y 坐标
        /// @param theta 旋转角度（弧度）
        void rotate(const f32 cx, const f32 cy, const f32 theta) override {
            for (auto &vertex: m_vertices) {
                vertex.rotate(cx, cy, theta);
            }
        }

        /// @brief 将图形写入批处理渲染器
        /// @param batchRenderer 批处理渲染器引用
        void writeToBatch(BatchRenderer &batchRenderer) const override {
            auto vertices = batchRenderer.allocateVertices(0, (m_vertices.size() - 2) * 3);
            for (i32 i = 1; i < VertexNum - 1; ++i) {
                *(vertices++) = m_vertices[0].getSDLVertex();
                *(vertices++) = m_vertices[i].getSDLVertex();
                *(vertices++) = m_vertices[i + 1].getSDLVertex();
            }
        }

    private:
        std::array<Vertex, VertexNum> m_vertices; ///< 多边形顶点数组
    };
}