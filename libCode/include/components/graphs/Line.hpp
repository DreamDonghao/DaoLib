#pragma once
#include <core/tool/type.hpp>
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/Vertex.hpp>
#include <components/graphs/IGraph.hpp>
#include <cmath>

namespace dao {
    /// @brief 线段图形组件
    /// @details 由起点和终点定义的线段，通过垂直方向扩展厚度生成四边形，以 2 个三角形方式渲染
    class Line : public ifc::IGraph {
    public:
        /// @brief 构造函数
        /// @param x1 起点 x 坐标
        /// @param y1 起点 y 坐标
        /// @param x2 终点 x 坐标
        /// @param y2 终点 y 坐标
        /// @param thickness 线条厚度，默认为 1.0
        /// @param color 填充颜色，默认为白色
        Line(f32 x1, f32 y1, f32 x2, f32 y2,
             f32 thickness = 1.0f, const ColorRGBA &color = White);

        /// @brief 设置起点
        /// @param x 起点 x 坐标
        /// @param y 起点 y 坐标
        void setStart(f32 x, f32 y);

        /// @brief 设置终点
        /// @param x 终点 x 坐标
        /// @param y 终点 y 坐标
        void setEnd(f32 x, f32 y);

        /// @brief 设置厚度
        /// @param thickness 线条厚度
        void setThickness(f32 thickness);

        /// @brief 设置颜色
        /// @param color 填充颜色
        void setColor(const ColorRGBA &color);

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
        f32 m_x1;        ///< 起点 x 坐标
        f32 m_y1;        ///< 起点 y 坐标
        f32 m_x2;        ///< 终点 x 坐标
        f32 m_y2;        ///< 终点 y 坐标
        f32 m_thickness; ///< 线条厚度

        std::vector<Vertex> m_vertices; ///< 4 个顶点构成的线段矩形

        /// @brief 重新计算所有顶点位置
        /// @param color 顶点颜色
        void updateVertices(const ColorRGBA &color);
    };
}