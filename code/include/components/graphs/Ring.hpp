#pragma once
#include <core/tool/type.hpp>
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/Vertex.hpp>
#include <components/graphs/IGraph.hpp>
#include <cmath>

namespace dao {
    /// @brief 圆环图形组件
    /// @details 由圆心、内外半径定义的圆环（环形）
    class Ring : public ifc::IGraph {
    public:
        /// @brief 构造函数
        /// @param cx 圆心 x 坐标
        /// @param cy 圆心 y 坐标
        /// @param innerRadius 内半径
        /// @param outerRadius 外半径
        /// @param color 填充颜色
        /// @param segments 分段数（默认 32，越大越平滑）
        Ring(const f32 cx, const f32 cy, const f32 innerRadius, const f32 outerRadius,
             const ColorRGBA &color = White, const i32 segments = 32)
            : m_cx(cx), m_cy(cy), m_innerRadius(innerRadius), m_outerRadius(outerRadius),
              m_segments(segments) {
            updateVertices(color);
        }

        /// @brief 设置圆心位置
        void setPosition(const f32 cx, const f32 cy) {
            const f32 dx = cx - m_cx;
            const f32 dy = cy - m_cy;
            translate(dx, dy);
        }

        /// @brief 设置内半径
        void setInnerRadius(const f32 innerRadius) {
            m_innerRadius = innerRadius;
            updateVertices(m_vertices.empty() ? White : m_vertices[0].color());
        }

        /// @brief 设置外半径
        void setOuterRadius(const f32 outerRadius) {
            m_outerRadius = outerRadius;
            updateVertices(m_vertices.empty() ? White : m_vertices[0].color());
        }

        /// @brief 设置颜色
        void setColor(const ColorRGBA &color) {
            for (auto &vertex: m_vertices) {
                vertex.setColor(color);
            }
        }

        /// @brief 设置分段数
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
            std::tie(m_cx, m_cy) = rotatePointRad(m_cx, m_cy, cx, cy, theta);
            for (auto &vertex: m_vertices) {
                vertex.rotate(cx, cy, theta);
            }
        }

        void writeToBatch(BatchRenderer &batchRenderer) const override {
            // 圆环由 segments 个四边形组成，每个四边形 2 个三角形 = 6 个顶点
            auto vertices = batchRenderer.allocateVertices(0, m_segments * 6);
            for (i32 i = 0; i < m_segments; ++i) {
                const i32 next = (i + 1) % m_segments;
                // 两个三角形组成一个四边形段
                *vertices++ = m_vertices[i].getSDLVertex();               // 内圈当前点
                *vertices++ = m_vertices[i + m_segments + 1].getSDLVertex(); // 外圈当前点
                *vertices++ = m_vertices[next].getSDLVertex();            // 内圈下一点
                *vertices++ = m_vertices[i + m_segments + 1].getSDLVertex(); // 外圈当前点
                *vertices++ = m_vertices[next + m_segments + 1].getSDLVertex(); // 外圈下一点
                *vertices++ = m_vertices[next].getSDLVertex();            // 内圈下一点
            }
        }

    private:
        f32 m_cx;          ///< 圆心 x 坐标
        f32 m_cy;          ///< 圆心 y 坐标
        f32 m_innerRadius; ///< 内半径
        f32 m_outerRadius; ///< 外半径
        i32 m_segments;    ///< 分段数

        std::vector<Vertex> m_vertices; ///< 顶点：[0..segments]=内圈, [segments+1..2*segments+1]=外圈

        /// @brief 更新顶点位置
        void updateVertices(const ColorRGBA &color) {
            // 内圈 segments+1 个点，外圈 segments+1 个点
            m_vertices.resize((m_segments + 1) * 2);

            const f32 angleStep = 2.0f * f32pi / static_cast<f32>(m_segments);
            for (i32 i = 0; i <= m_segments; ++i) {
                const f32 angle = i * angleStep;
                const f32 cosA = std::cos(angle);
                const f32 sinA = std::sin(angle);

                // 内圈顶点
                m_vertices[i] = Vertex(
                    m_cx + m_innerRadius * cosA,
                    m_cy + m_innerRadius * sinA,
                    color);
                // 外圈顶点
                m_vertices[i + m_segments + 1] = Vertex(
                    m_cx + m_outerRadius * cosA,
                    m_cy + m_outerRadius * sinA,
                    color);
            }
        }
    };
}