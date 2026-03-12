#pragma once
#include <core/tool/type.hpp>
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/Vertex.hpp>
#include <components/graphs/IGraph.hpp>
#include <cmath>

namespace dao {
    /// @brief 圆角矩形图形组件
    /// @details 带圆角的矩形，由位置、尺寸和圆角半径定义
    class RoundedRectangle : public ifc::IGraph {
    public:
        /// @brief 构造函数
        /// @param x 左上角 x 坐标
        /// @param y 左上角 y 坐标
        /// @param width 宽度
        /// @param height 高度
        /// @param cornerRadius 圆角半径
        /// @param color 填充颜色
        /// @param cornerSegments 每个圆角的分段数（默认 8）
        RoundedRectangle(const f32 x, const f32 y, const f32 width, const f32 height,
                         const f32 cornerRadius, const ColorRGBA &color = White,
                         const i32 cornerSegments = 8)
            : m_x(x), m_y(y), m_width(width), m_height(height),
              m_cornerRadius(cornerRadius), m_cornerSegments(cornerSegments), m_color(color) {
            updateVertices();
        }

        /// @brief 设置位置（左上角）
        void setPosition(const f32 x, const f32 y) {
            const f32 dx = x - m_x;
            const f32 dy = y - m_y;
            translate(dx, dy);
        }

        /// @brief 设置尺寸
        void setSize(const f32 width, const f32 height) {
            m_width = width;
            m_height = height;
            updateVertices();
        }

        /// @brief 设置圆角半径
        void setCornerRadius(const f32 radius) {
            m_cornerRadius = radius;
            updateVertices();
        }

        /// @brief 设置颜色
        void setColor(const ColorRGBA &color) {
            m_color = color;
            for (auto &vertex: m_vertices) {
                vertex.setColor(color);
            }
        }

        /// @brief 设置圆角分段数
        void setCornerSegments(const i32 segments) {
            m_cornerSegments = segments;
            updateVertices();
        }

        void translate(const f32 dx, const f32 dy) override {
            m_x += dx;
            m_y += dy;
            for (auto &vertex: m_vertices) {
                vertex.translate(dx, dy);
            }
        }

        void rotate(const f32 cx, const f32 cy, const f32 theta) override {
            for (auto &vertex: m_vertices) {
                vertex.rotate(cx, cy, theta);
            }
            m_x = m_vertices[0].x();
            m_y = m_vertices[0].y();
        }

        void writeToBatch(BatchRenderer &batchRenderer) const override {
            const i32 vertexCount = static_cast<i32>(m_vertices.size());
            auto vertices = batchRenderer.allocateVertices(0, (vertexCount - 1) * 3);
            i32 i;
            for (i = 1; i < vertexCount - 1; ++i) {
                *vertices++ = m_vertices[0].getSDLVertex();     // 中心点
                *vertices++ = m_vertices[i].getSDLVertex();     // 当前轮廓点
                *vertices++ = m_vertices[i + 1].getSDLVertex(); // 下一个轮廓点
            }
            *vertices++ = m_vertices[0].getSDLVertex();     // 中心点
            *vertices++ = m_vertices[i].getSDLVertex();     // 当前轮廓点
            *vertices++ = m_vertices[1].getSDLVertex(); // 下一个轮廓点
        }

    private:
        f32 m_x;              ///< 左上角 x 坐标
        f32 m_y;              ///< 左上角 y 坐标
        f32 m_width;          ///< 宽度
        f32 m_height;         ///< 高度·
        f32 m_cornerRadius;   ///< 圆角半径
        i32 m_cornerSegments; ///< 每个圆角的分段数
        ColorRGBA m_color;    ///< 当前颜色

        std::vector<Vertex> m_vertices; ///< 顶点列表：[0]=中心点，[1..n]=轮廓点

        /// @brief 更新顶点位置
        void updateVertices() {
            const f32 maxRadius = std::min(m_width, m_height) / 2.0f;
            const f32 radius = std::min(m_cornerRadius, maxRadius);

            const f32 right = m_x + m_width;
            const f32 bottom = m_y + m_height;

            const i32 cornerPointCount = m_cornerSegments + 1;
            const i32 totalVertices = 1 + 4 * cornerPointCount;
            m_vertices.resize(totalVertices);

            const f32 centerX = m_x + m_width / 2.0f;
            const f32 centerY = m_y + m_height / 2.0f;
            m_vertices[0] = Vertex(centerX, centerY, m_color);

            i32 vertexIndex = 1;
            const f32 angleStep = f32pi / 2.0f / static_cast<f32>(m_cornerSegments); // 使用 f32pi

            // 左上角圆角
            const f32 lt_cx = m_x + radius;
            const f32 lt_cy = m_y + radius;
            for (i32 i = 0; i <= m_cornerSegments; ++i) {
                const f32 angle = f32pi + i * angleStep; // 从 180° 到 270°
                m_vertices[vertexIndex++] = Vertex(
                    lt_cx + radius * std::cos(angle),
                    lt_cy + radius * std::sin(angle),
                    m_color);
            }

            // 右上角圆角
            const f32 rt_cx = right - radius;
            const f32 rt_cy = m_y + radius;
            for (i32 i = 0; i <= m_cornerSegments; ++i) {
                const f32 angle = -f32pi / 2.0f + i * angleStep; // 从 -90° 到 0°
                m_vertices[vertexIndex++] = Vertex(
                    rt_cx + radius * std::cos(angle),
                    rt_cy + radius * std::sin(angle),
                    m_color);
            }

            // 右下角圆角
            const f32 rb_cx = right - radius;
            const f32 rb_cy = bottom - radius;
            for (i32 i = 0; i <= m_cornerSegments; ++i) {
                const f32 angle = i * angleStep; // 从 0° 到 90°
                m_vertices[vertexIndex++] = Vertex(
                    rb_cx + radius * std::cos(angle),
                    rb_cy + radius * std::sin(angle),
                    m_color);
            }

            // 左下角圆角
            const f32 lb_cx = m_x + radius;
            const f32 lb_cy = bottom - radius;
            for (i32 i = 0; i <= m_cornerSegments; ++i) {
                const f32 angle = f32pi / 2.0f + i * angleStep; // 从 90° 到 180°
                m_vertices[vertexIndex++] = Vertex(
                    lb_cx + radius * std::cos(angle),
                    lb_cy + radius * std::sin(angle),
                    m_color);
            }
        }
    };
}