#pragma once
#include <core/tool/type.hpp>
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/Vertex.hpp>
#include <components/graphs/IGraph.hpp>
#include <cmath>

namespace dao {
    /// @brief 圆弧图形组件
    /// @details 从起始角度到结束角度的圆弧，通过离散化线段进行渲染
    class Arc : public ifc::IGraph {
    public:
        /// @brief 构造函数
        /// @param cx 圆心 x 坐标
        /// @param cy 圆心 y 坐标
        /// @param radius 半径
        /// @param startAngle 起始角度（弧度）
        /// @param endAngle 结束角度（弧度）
        /// @param thickness 线条厚度
        /// @param color 填充颜色
        /// @param segments 分段数（默认 32，越大越平滑）
        Arc(const f32 cx, const f32 cy, const f32 radius,
            const f32 startAngle, const f32 endAngle,
            const f32 thickness = 1.0f, const ColorRGBA &color = White,
            const i32 segments = 32)
            : m_cx(cx), m_cy(cy), m_radius(radius),
              m_startAngle(startAngle), m_endAngle(endAngle),
              m_thickness(thickness), m_segments(segments) {
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
            updateVertices(m_vertices.empty() ? White : m_vertices[0].color());
        }

        /// @brief 设置角度范围
        void setAngles(const f32 startAngle, const f32 endAngle) {
            m_startAngle = startAngle;
            m_endAngle = endAngle;
            updateVertices(m_vertices.empty() ? White : m_vertices[0].color());
        }

        /// @brief 设置厚度
        void setThickness(const f32 thickness) {
            m_thickness = thickness;
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
            m_startAngle += theta;
            m_endAngle += theta;
            updateVertices(m_vertices.empty() ? White : m_vertices[0].color());
        }

        void writeToBatch(BatchRenderer &batchRenderer) const override {
            // 每个分段是 2 个三角形 = 6 个顶点
            auto vertices = batchRenderer.allocateVertices(0, m_segments * 6);
            for (i32 i = 0; i < m_segments; ++i) {
                *vertices++ = m_vertices[i * 2].getSDLVertex();
                *vertices++ = m_vertices[i * 2 + 1].getSDLVertex();
                *vertices++ = m_vertices[i * 2 + 2].getSDLVertex();
                *vertices++ = m_vertices[i * 2 + 1].getSDLVertex();
                *vertices++ = m_vertices[i * 2 + 3].getSDLVertex();
                *vertices++ = m_vertices[i * 2 + 2].getSDLVertex();
            }
        }

    private:
        f32 m_cx;         ///< 圆心 x 坐标
        f32 m_cy;         ///< 圆心 y 坐标
        f32 m_radius;     ///< 半径
        f32 m_startAngle; ///< 起始角度（弧度）
        f32 m_endAngle;   ///< 结束角度（弧度）
        f32 m_thickness;  ///< 线条厚度
        i32 m_segments;   ///< 分段数

        std::vector<Vertex> m_vertices; ///< 顶点：每段4个顶点

        /// @brief 更新顶点位置
        void updateVertices(const ColorRGBA &color) {
            // 每段需要4个顶点（内外各2个）
            m_vertices.resize((m_segments + 1) * 2);

            const f32 angleStep = (m_endAngle - m_startAngle) / static_cast<f32>(m_segments);
            const f32 innerR = m_radius - m_thickness / 2.0f;
            const f32 outerR = m_radius + m_thickness / 2.0f;

            for (i32 i = 0; i <= m_segments; ++i) {
                const f32 angle = m_startAngle + i * angleStep;
                const f32 cosA = std::cos(angle);
                const f32 sinA = std::sin(angle);

                m_vertices[i * 2] = Vertex(
                    m_cx + innerR * cosA,
                    m_cy + innerR * sinA,
                    color);
                m_vertices[i * 2 + 1] = Vertex(
                    m_cx + outerR * cosA,
                    m_cy + outerR * sinA,
                    color);
            }
        }
    };
}