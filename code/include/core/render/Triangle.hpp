#pragma once

#include <array>
#include <core/render/primitives/Vertex.hpp>
#include <core/tool/better_stl.hpp>
#include <core/render/primitives/ColorRGB.hpp>

namespace dao {
    /// @brief 三角形几何图元
    /// @details 表示一个由三个顶点组成的三角形，支持设置位置、颜色等属性
    /// 提供按索引和按顶点名称（A、B、C）两种访问方式，便于灵活操作
    class Triangle {
    public:
        /// @brief 顶点索引枚举
        enum VertexIndex : size_t {
            A = 0, ///< 顶点A索引
            B = 1, ///< 顶点B索引
            C = 2  ///< 顶点C索引
        };

        /// @brief 默认构造函数，创建白色三角形（三个顶点均在原点）
        Triangle() : m_vertices{Vertex{}, Vertex{}, Vertex{}} {
        }

        /// @brief 通过三个顶点构造三角形
        /// @param vertexA 顶点A
        /// @param vertexB 顶点B
        /// @param vertexC 顶点C
        Triangle(const Vertex &vertexA, const Vertex &vertexB, const Vertex &vertexC)
            : m_vertices({vertexA, vertexB, vertexC}) {
        }

        /// @brief 通过索引设置顶点属性（位置和颜色）
        /// @param index 顶点索引（0=A, 1=B, 2=C），建议使用VertexIndex枚举
        /// @param x 新的x坐标
        /// @param y 新的y坐标
        /// @param color 新的颜色
        void setVertex(const size_t index, const f32 x, const f32 y, const ColorRGBA &color) {
            if (index < 3) m_vertices[index].set(x, y, color);
        }

        /// @brief 通过索引设置顶点对象
        /// @param index 顶点索引（0=A, 1=B, 2=C），建议使用VertexIndex枚举
        /// @param vertex 新的顶点对象
        void setVertex(const size_t index, const Vertex &vertex) {
            if (index < 3) m_vertices[index] = vertex;
        }

        /// @brief 通过索引设置顶点位置（保持颜色不变）
        /// @param index 顶点索引（0=A, 1=B, 2=C），建议使用VertexIndex枚举
        /// @param x 新的x坐标
        /// @param y 新的y坐标
        void setVertexPosition(const size_t index, const f32 x, const f32 y) {
            if (index < 3) m_vertices[index].setPosition(x, y);
        }

        /// @brief 通过索引设置顶点颜色（保持位置不变）
        /// @param index 顶点索引（0=A, 1=B, 2=C），建议使用VertexIndex枚举
        /// @param color 新的颜色
        void setVertexColor(const size_t index, const ColorRGBA &color) {
            if (index < 3) m_vertices[index].setColor(color);
        }

        /// @brief 通过索引获取顶点
        /// @param index 顶点索引（0=A, 1=B, 2=C），建议使用VertexIndex枚举
        /// @return 对应索引的顶点常量引用
        [[nodiscard]] const Vertex &vertex(const size_t index) const {
            return m_vertices[index];
        }

        /// @brief 设置所有顶点颜色（保持位置不变）
        /// @param color 新的颜色
        void setAllColors(const ColorRGBA &color) {
            for (auto &vertex : m_vertices) {
                vertex.setColor(color);
            }
        }

        /// @brief 平移整个三角形
        /// @param dx x方向平移量
        /// @param dy y方向平移量
        void translate(const f32 dx, const f32 dy) {
            for (auto &vertex : m_vertices) {
                vertex.setPosition(vertex.x() + dx, vertex.y() + dy);
            }
        }

        /// @brief 检查三角形是否有效（三个顶点不共线）
        /// @note 当前为简单实现，仅检查顶点是否不同
        /// @return 如果三个顶点不同返回true，否则false
        [[nodiscard]] bool isValid() const {
            const auto &v0 = m_vertices[0];
            const auto &v1 = m_vertices[1];
            const auto &v2 = m_vertices[2];

            // 简单检查：三个顶点不应完全相同
            return !(v0.x() == v1.x() && v0.y() == v1.y() &&
                     v1.x() == v2.x() && v1.y() == v2.y());
        }

    private:
        std::array<Vertex, 3> m_vertices; ///< 三角形的三个顶点
    };
}