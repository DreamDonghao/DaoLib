#pragma once
#include <core/tool/type.hpp>
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/Vertex.hpp>
#include <components/graphs/IGraph.hpp>
#include <cmath>

namespace dao {
    /// @brief 扇形图形组件
    /// @details 由圆心、半径和角度范围定义的扇形
    class Sector : public ifc::IGraph {
    public:
        /// @brief 构造函数
        /// @param cx 圆心 x 坐标
        /// @param cy 圆心 y 坐标
        /// @param radius 半径
        /// @param startAngle 起始角度（弧度）
        /// @param endAngle 结束角度（弧度）
        /// @param color 填充颜色
        /// @param segments 分段数（默认 32，越大越平滑）
        Sector(const f32 cx, const f32 cy, const f32 radius,
               const f32 startAngle, const f32 endAngle,
               const ColorRGBA &color = White, const i32 segments = 32)
            : m_cx(cx), m_cy(cy), m_radius(radius),
              m_startAngle(startAngle), m_endAngle(endAngle), m_segments(segments) {
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
            // 扇形由 (segments) 个三角形组成
            auto vertices = batchRenderer.allocateVertices(0, m_segments * 3);
            for (i32 i = 0; i < m_segments; ++i) {
                *vertices++ = m_vertices[0].getSDLVertex();           // 圆心
                *vertices++ = m_vertices[i + 1].getSDLVertex();       // 当前边缘点
                *vertices++ = m_vertices[i + 2].getSDLVertex();       // 下一个边缘点
            }
        }

    private:
        f32 m_cx;         ///< 圆心 x 坐标
        f32 m_cy;         ///< 圆心 y 坐标
        f32 m_radius;     ///< 半径
        f32 m_startAngle; ///< 起始角度（弧度）
        f32 m_endAngle;   ///< 结束角度（弧度）
        i32 m_segments;   ///< 分段数

        std::vector<Vertex> m_vertices; ///< 顶点：[0]=圆心, [1..segments+1]=边缘点

        /// @brief 更新顶点位置
        void updateVertices(const ColorRGBA &color) {
            m_vertices.resize(m_segments + 2);
            m_vertices[0] = Vertex(m_cx, m_cy, color); // 圆心

            const f32 angleStep = (m_endAngle - m_startAngle) / static_cast<f32>(m_segments);
            for (i32 i = 0; i <= m_segments; ++i) {
                const f32 angle = m_startAngle + i * angleStep;
                const f32 x = m_cx + m_radius * std::cos(angle);
                const f32 y = m_cy + m_radius * std::sin(angle);
                m_vertices[i + 1] = Vertex(x, y, color);
            }
        }
    };
}