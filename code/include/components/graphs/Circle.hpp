#pragma once
#include <core/tool/type.hpp>
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/Vertex.hpp>
#include <components/graphs/IGraph.hpp>
#include <cmath>

namespace dao {
    /// @brief 圆形图形组件
    /// @details 通过将圆离散化为多边形进行渲染，分段数决定圆的平滑度
    class Circle : public ifc::IGraph {
    public:
        /// @brief 构造函数
        /// @param cx 圆心 x 坐标
        /// @param cy 圆心 y 坐标
        /// @param radius 半径
        /// @param color 填充颜色
        /// @param segments 分段数（默认 32，越大越平滑）
        Circle(const f32 cx, const f32 cy, const f32 radius, const ColorRGBA &color = White, const i32 segments = 32)
            : m_cx(cx), m_cy(cy), m_radius(radius), m_segments(segments) {
            updateVertices(color);
        }

        /// @brief 设置圆心位置
        void setPosition(const f32 cx, const f32 cy) {
            const f32 dx = cx - m_cx;
            const f32 dy = cy - m_cy;
            translate(dx, dy);
        }

        /// @brief 设置半径
        void setRadius(const f32 radius) {
            m_radius = radius;
        }

        /// @brief 设置颜色
        void setColor(const ColorRGBA color) {
            for (auto &vertex: m_vertices) {
                vertex.setColor(color);
            }
        }

        /// @brief 设置分段数（需要重新生成顶点）
        void setSegments(const i32 segments) {
            m_segments = segments;
            updateVertices(m_vertices.empty() ? White : m_vertices[0].color());
        }

        void translate(const f32 dx, const f32 dy) override {
            m_cx += dx;
            m_cy += dy;
            for (auto &vertex: m_vertices) {
                vertex.translate(dx, dy);
            }
        }

        void rotate(const f32 cx, const f32 cy, const f32 theta) override {
            // 旋转圆心
            std::tie(m_cx, m_cy) = rotatePointRad(m_cx, m_cy, cx, cy, theta);
            // 旋转所有边缘顶点
            for (auto &vertex: m_vertices) {
                vertex.rotate(cx, cy, theta);
            }
        }

        void writeToBatch(BatchRenderer &batchRenderer) const override {
            // 圆由 (segments) 个三角形组成，每个三角形 3 个顶点
            auto vertices = batchRenderer.allocateVertices(0, m_segments * 3);
            for (i32 i = 0; i < m_segments; ++i) {
                *vertices++ = m_vertices[0].getSDLVertex();                        // 圆心
                *vertices++ = m_vertices[i + 1].getSDLVertex();                    // 当前边缘点
                *vertices++ = m_vertices[(i + 1) % m_segments + 1].getSDLVertex(); // 下一个边缘点
            }
        }

    private:
        f32 m_cx;       ///< 圆心 x 坐标
        f32 m_cy;       ///< 圆心 y 坐标
        f32 m_radius;   ///< 半径
        i32 m_segments; ///< 分段数

        std::vector<Vertex> m_vertices; ///< 顶点：[0]=圆心，[1..segments]=边缘点

        /// @brief 更新顶点位置
        void updateVertices(const ColorRGBA &color) {
            m_vertices.resize(m_segments + 1);
            m_vertices[0] = Vertex(m_cx, m_cy, color); // 圆心

            const f32 angleStep = 2.0f * f32pi / m_segments;
            for (i32 i = 0; i < m_segments; ++i) {
                const f32 angle = i * angleStep;
                const f32 x = m_cx + m_radius * std::cos(angle);
                const f32 y = m_cy + m_radius * std::sin(angle);
                m_vertices[i + 1] = Vertex(x, y, color);
            }
        }
    };
}
