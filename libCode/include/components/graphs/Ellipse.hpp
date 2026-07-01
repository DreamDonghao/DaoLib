#pragma once
#include <core/tool/type.hpp>
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/Vertex.hpp>
#include <components/graphs/IGraph.hpp>

namespace dao {
    /// @brief 椭圆图形组件
    /// @details 以中心点为共同顶点、边缘点为底的三角形扇形方式渲染，分段数决定椭圆的平滑度
    class Ellipse : public ifc::IGraph {
    public:
        /// @brief 构造函数
        /// @param cx 中心 x 坐标
        /// @param cy 中心 y 坐标
        /// @param radiusX x 方向半径
        /// @param radiusY y 方向半径
        /// @param color 填充颜色，默认为白色
        /// @param segments 分段数，默认 32，越大越平滑
        Ellipse(f32 cx, f32 cy, f32 radiusX, f32 radiusY,
                const ColorRGBA &color = White, i32 segments = 32);

        /// @brief 设置中心位置
        /// @param cx 中心 x 坐标
        /// @param cy 中心 y 坐标
        void setPosition(f32 cx, f32 cy);

        /// @brief 设置半径
        /// @param radiusX x 方向半径
        /// @param radiusY y 方向半径
        void setRadius(f32 radiusX, f32 radiusY);

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
        f32 m_cx;       ///< 中心 x 坐标
        f32 m_cy;       ///< 中心 y 坐标
        f32 m_radiusX;  ///< x 方向半径
        f32 m_radiusY;  ///< y 方向半径
        i32 m_segments; ///< 分段数

        std::vector<Vertex> m_vertices; ///< 顶点数组：[0]=中心，[1..segments]=边缘点

        /// @brief 重新计算所有顶点位置
        /// @param color 顶点颜色
        void updateVertices(const ColorRGBA &color);
    };
}