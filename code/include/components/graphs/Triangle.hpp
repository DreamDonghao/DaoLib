#pragma once
#include <core/tool/type.hpp>
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/Vertex.hpp>
#include <components/graphs/IGraph.hpp>
#include <cmath>

namespace dao {
    /// @brief 三角形图形组件
    /// @details 由三个顶点定义的三角形
    class Triangle : public ifc::IGraph {
    public:
        /// @brief 构造函数
        /// @param x1 第一个顶点 x 坐标
        /// @param y1 第一个顶点 y 坐标
        /// @param x2 第二个顶点 x 坐标
        /// @param y2 第二个顶点 y 坐标
        /// @param x3 第三个顶点 x 坐标
        /// @param y3 第三个顶点 y 坐标
        /// @param color 填充颜色
        Triangle(const f32 x1, const f32 y1, const f32 x2, const f32 y2,
                 const f32 x3, const f32 y3, const ColorRGBA &color = White) {
            m_vertices[0] = Vertex(x1, y1, color);
            m_vertices[1] = Vertex(x2, y2, color);
            m_vertices[2] = Vertex(x3, y3, color);
        }

        /// @brief 设置顶点位置
        /// @param index 顶点索引 (0, 1, 2)
        void setVertex(const i32 index, const f32 x, const f32 y) {
            if (index >= 0 && index < 3) {
                m_vertices[index].setPosition(x, y);
            }
        }

        /// @brief 设置颜色
        void setColor(const ColorRGBA &color) {
            for (auto &vertex: m_vertices) {
                vertex.setColor(color);
            }
        }

        /// @brief 获取顶点 x 坐标
        /// @param index 顶点索引 (0, 1, 2)
        [[nodiscard]] f32 vertexX(const i32 index) const {
            return m_vertices[index].x();
        }

        /// @brief 获取顶点 y 坐标
        /// @param index 顶点索引 (0, 1, 2)
        [[nodiscard]] f32 vertexY(const i32 index) const {
            return m_vertices[index].y();
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
            auto vertices = batchRenderer.allocateVertices(0, 3);
            *vertices++ = m_vertices[0].getSDLVertex();
            *vertices++ = m_vertices[1].getSDLVertex();
            *vertices = m_vertices[2].getSDLVertex();
        }

    private:
        std::array<Vertex, 3> m_vertices; ///< 三个顶点
    };
}