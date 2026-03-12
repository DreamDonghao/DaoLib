#pragma once
#include <core/tool/type.hpp>
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/Vertex.hpp>
#include <components/graphs/IGraph.hpp>
#include <cmath>

namespace dao {
    /// @brief 矩形图形组件
    /// @details 轴对齐矩形，由左上角位置、宽度和高度定义
    class Rectangle : public ifc::IGraph {
    public:
        /// @brief 构造函数
        /// @param x 左上角 x 坐标
        /// @param y 左上角 y 坐标
        /// @param width 宽度
        /// @param height 高度
        /// @param color 填充颜色
        Rectangle(const f32 x, const f32 y, const f32 width, const f32 height,
                  const ColorRGBA &color = White)
            : m_x(x), m_y(y), m_width(width), m_height(height) {
            updateVertices(color);
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
            updateVertices(m_vertices.empty() ? White : m_vertices[0].color());
        }

        /// @brief 设置颜色
        void setColor(const ColorRGBA &color) {
            for (auto &vertex: m_vertices) {
                vertex.setColor(color);
            }
        }

        /// @brief 获取 x 坐标
        [[nodiscard]] f32 x() const noexcept { return m_x; }

        /// @brief 获取 y 坐标
        [[nodiscard]] f32 y() const noexcept { return m_y; }

        /// @brief 获取宽度
        [[nodiscard]] f32 width() const noexcept { return m_width; }

        /// @brief 获取高度
        [[nodiscard]] f32 height() const noexcept { return m_height; }

        void translate(const f32 dx, const f32 dy) override {
            m_x += dx;
            m_y += dy;
            for (auto &vertex: m_vertices) {
                vertex.translate(dx, dy);
            }
        }

        void rotate(const f32 cx, const f32 cy, const f32 theta) override {
            // 旋转四个角点
            for (auto &vertex: m_vertices) {
                vertex.rotate(cx, cy, theta);
            }
            // 更新左上角位置为新的最小点
            m_x = m_vertices[0].x();
            m_y = m_vertices[0].y();
        }

        void writeToBatch(BatchRenderer &batchRenderer) const override {
            // 矩形由 2 个三角形组成，每个三角形 3 个顶点
            auto vertices = batchRenderer.allocateVertices(0, 6);
            // 第一个三角形
            *vertices++ = m_vertices[0].getSDLVertex(); // 左上
            *vertices++ = m_vertices[1].getSDLVertex(); // 右上
            *vertices++ = m_vertices[2].getSDLVertex(); // 左下
            // 第二个三角形
            *vertices++ = m_vertices[1].getSDLVertex(); // 右上
            *vertices++ = m_vertices[3].getSDLVertex(); // 右下
            *vertices++ = m_vertices[2].getSDLVertex(); // 左下
        }

    private:
        f32 m_x;      ///< 左上角 x 坐标
        f32 m_y;      ///< 左上角 y 坐标
        f32 m_width;  ///< 宽度
        f32 m_height; ///< 高度

        std::vector<Vertex> m_vertices; ///< 顶点：[0]=左上, [1]=右上, [2]=左下, [3]=右下

        /// @brief 更新顶点
        void updateVertices(const ColorRGBA &color) {
            m_vertices.resize(4);
            m_vertices[0] = Vertex(m_x, m_y, color);                         // 左上
            m_vertices[1] = Vertex(m_x + m_width, m_y, color);               // 右上
            m_vertices[2] = Vertex(m_x, m_y + m_height, color);              // 左下
            m_vertices[3] = Vertex(m_x + m_width, m_y + m_height, color);    // 右下
        }
    };
}