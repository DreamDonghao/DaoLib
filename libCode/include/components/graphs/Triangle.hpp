#pragma once
#include <core/tool/type.hpp>
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/Vertex.hpp>
#include <components/graphs/IGraph.hpp>

namespace dao {
    /// @brief 三角形图形组件
    /// @details 由三个顶点定义的单三角形
    class Triangle : public ifc::IGraph {
    public:
        /// @brief 构造函数
        /// @param x1 第一个顶点 x 坐标
        /// @param y1 第一个顶点 y 坐标
        /// @param x2 第二个顶点 x 坐标
        /// @param y2 第二个顶点 y 坐标
        /// @param x3 第三个顶点 x 坐标
        /// @param y3 第三个顶点 y 坐标
        /// @param color 填充颜色，默认为白色
        Triangle(f32 x1, f32 y1, f32 x2, f32 y2,
                 f32 x3, f32 y3, const ColorRGBA &color = White);

        /// @brief 设置指定顶点的位置
        /// @param index 顶点索引（0, 1, 2），越界则忽略
        /// @param x 顶点 x 坐标
        /// @param y 顶点 y 坐标
        void setVertex(i32 index, f32 x, f32 y);

        /// @brief 设置颜色
        /// @param color 填充颜色
        void setColor(const ColorRGBA &color);

        /// @brief 获取指定顶点的 x 坐标
        /// @param index 顶点索引（0, 1, 2）
        /// @returns 顶点 x 坐标
        [[nodiscard]] f32 vertexX(i32 index) const { return m_vertices[index].x(); }

        /// @brief 获取指定顶点的 y 坐标
        /// @param index 顶点索引（0, 1, 2）
        /// @returns 顶点 y 坐标
        [[nodiscard]] f32 vertexY(i32 index) const { return m_vertices[index].y(); }

        /// @brief 平移图形
        /// @param dx x 方向平移量
        /// @param dy y 方向平移量
        void translate(f32 dx, f32 dy) override;

        /// @brief 旋转图形
        /// @param cx 旋转中心 x 坐标
        /// @param cy 旋转中心 y 坐标
        /// @param theta 旋转角度（弧度）
        void rotate(f32 cx, f32 cy, f32 theta) override;

        /// @brief 将图形写入批处理渲染器
        /// @param batchRenderer 批处理渲染器引用
        void writeToBatch(BatchRenderer &batchRenderer) const override;

    private:
        std::array<Vertex, 3> m_vertices; ///< 三个顶点
    };
}