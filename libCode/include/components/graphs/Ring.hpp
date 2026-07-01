#pragma once
#include <core/tool/type.hpp>
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/Vertex.hpp>
#include <components/graphs/IGraph.hpp>

namespace dao {
    /// @brief 圆环图形组件
    /// @details 由圆心、内外半径定义的圆环（环形），以分段四边形条带方式渲染
    class Ring : public ifc::IGraph {
    public:
        /// @brief 构造函数
        /// @param cx 圆心 x 坐标
        /// @param cy 圆心 y 坐标
        /// @param innerRadius 内半径
        /// @param outerRadius 外半径
        /// @param color 填充颜色，默认为白色
        /// @param segments 分段数，默认 32，越大越平滑
        Ring(f32 cx, f32 cy, f32 innerRadius, f32 outerRadius,
             const ColorRGBA &color = White, i32 segments = 64);

        /// @brief 设置圆心位置
        /// @param cx 圆心 x 坐标
        /// @param cy 圆心 y 坐标
        void setPosition(f32 cx, f32 cy);

        /// @brief 设置内半径
        /// @param innerRadius 内半径
        void setInnerRadius(f32 innerRadius);

        /// @brief 设置外半径
        /// @param outerRadius 外半径
        void setOuterRadius(f32 outerRadius);

        /// @brief 设置颜色
        /// @param color 填充颜色
        void setColor(const ColorRGBA &color);

        /// @brief 设置分段数
        /// @param segments 分段数，越大越平滑
        void setSegments(i32 segments);

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
        f32 m_cx;          ///< 圆心 x 坐标
        f32 m_cy;          ///< 圆心 y 坐标
        f32 m_innerRadius; ///< 内半径
        f32 m_outerRadius; ///< 外半径
        i32 m_segments;    ///< 分段数

        std::vector<Vertex> m_vertices; ///< 顶点数组：[0..segments]=内圈，[segments+1..2*segments+1]=外圈

        /// @brief 重新计算所有顶点位置
        /// @param color 顶点颜色
        void updateVertices(const ColorRGBA &color);
    };
}