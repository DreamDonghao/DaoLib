#pragma once
#include <core/tool/type.hpp>
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/Vertex.hpp>
#include <components/graphs/IGraph.hpp>
#include <cmath>

namespace dao {
    /// @brief 线段图形组件
    /// @details 由起点和终点定义的线段，通过厚度扩展为多边形进行渲染
    class Line : public ifc::IGraph {
    public:
        /// @brief 构造函数
        /// @param x1 起点 x 坐标
        /// @param y1 起点 y 坐标
        /// @param x2 终点 x 坐标
        /// @param y2 终点 y 坐标
        /// @param thickness 线条厚度
        /// @param color 填充颜色
        Line(const f32 x1, const f32 y1, const f32 x2, const f32 y2,
             const f32 thickness = 1.0f, const ColorRGBA &color = White)
            : m_x1(x1), m_y1(y1), m_x2(x2), m_y2(y2), m_thickness(thickness) {
            updateVertices(color);
        }

        /// @brief 设置起点
        void setStart(const f32 x, const f32 y) {
            m_x1 = x;
            m_y1 = y;
            updateVertices(m_vertices.empty() ? White : m_vertices[0].color());
        }

        /// @brief 设置终点
        void setEnd(const f32 x, const f32 y) {
            m_x2 = x;
            m_y2 = y;
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

        void translate(const f32 dx, const f32 dy) override {
            m_x1 += dx;
            m_y1 += dy;
            m_x2 += dx;
            m_y2 += dy;
            for (auto &vertex: m_vertices) {
                vertex.translate(dx, dy);
            }
        }

        void rotate(const f32 cx, const f32 cy, const f32 theta) override {
            std::tie(m_x1, m_y1) = rotatePointRad(m_x1, m_y1, cx, cy, theta);
            std::tie(m_x2, m_y2) = rotatePointRad(m_x2, m_y2, cx, cy, theta);
            updateVertices(m_vertices.empty() ? White : m_vertices[0].color());
        }

        void writeToBatch(BatchRenderer &batchRenderer) const override {
            // 线段由 2 个三角形组成
            auto vertices = batchRenderer.allocateVertices(0, 6);
            *vertices++ = m_vertices[0].getSDLVertex();
            *vertices++ = m_vertices[1].getSDLVertex();
            *vertices++ = m_vertices[2].getSDLVertex();
            *vertices++ = m_vertices[1].getSDLVertex();
            *vertices++ = m_vertices[3].getSDLVertex();
            *vertices++ = m_vertices[2].getSDLVertex();
        }

    private:
        f32 m_x1;        ///< 起点 x 坐标
        f32 m_y1;        ///< 起点 y 坐标
        f32 m_x2;        ///< 终点 x 坐标
        f32 m_y2;        ///< 终点 y 坐标
        f32 m_thickness; ///< 线条厚度

        std::vector<Vertex> m_vertices; ///< 4个顶点构成线段矩形

        /// @brief 更新顶点位置
        void updateVertices(const ColorRGBA &color) {
            m_vertices.resize(4);

            // 计算线段方向向量
            const f32 dx = m_x2 - m_x1;
            const f32 dy = m_y2 - m_y1;
            const f32 len = std::sqrt(dx * dx + dy * dy);

            if (len < 1e-6f) {
                // 点线：生成一个正方形
                m_vertices[0] = Vertex(m_x1 - m_thickness / 2, m_y1 - m_thickness / 2, color);
                m_vertices[1] = Vertex(m_x1 + m_thickness / 2, m_y1 - m_thickness / 2, color);
                m_vertices[2] = Vertex(m_x1 - m_thickness / 2, m_y1 + m_thickness / 2, color);
                m_vertices[3] = Vertex(m_x1 + m_thickness / 2, m_y1 + m_thickness / 2, color);
                return;
            }

            // 计算垂直方向的单位向量
            const f32 nx = -dy / len;
            const f32 ny = dx / len;

            // 扩展厚度的一半
            const f32 half = m_thickness / 2.0f;
            const f32 offsetX = nx * half;
            const f32 offsetY = ny * half;

            // 四个顶点
            m_vertices[0] = Vertex(m_x1 + offsetX, m_y1 + offsetY, color);
            m_vertices[1] = Vertex(m_x1 - offsetX, m_y1 - offsetY, color);
            m_vertices[2] = Vertex(m_x2 + offsetX, m_y2 + offsetY, color);
            m_vertices[3] = Vertex(m_x2 - offsetX, m_y2 - offsetY, color);
        }
    };
}