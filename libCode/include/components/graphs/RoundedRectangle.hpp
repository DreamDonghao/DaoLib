#pragma once
#include <core/tool/type.hpp>
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/Vertex.hpp>
#include <components/graphs/IGraph.hpp>

namespace dao {
    /// @brief 圆角矩形图形组件
    /// @details 带四个圆角的矩形，以中心点为共同顶点的三角形扇形方式渲染。四个角分别用分段弧线逼近
    class RoundedRectangle : public ifc::IGraph {
    public:
        /// @brief 构造函数
        /// @param x 左上角 x 坐标
        /// @param y 左上角 y 坐标
        /// @param width 宽度
        /// @param height 高度
        /// @param cornerRadius 圆角半径
        /// @param color 填充颜色，默认为白色
        /// @param cornerSegments 每个圆角的分段数，默认 8，越大越平滑
        RoundedRectangle(f32 x, f32 y, f32 width, f32 height,
                         f32 cornerRadius, const ColorRGBA &color = White,
                         i32 cornerSegments = 8);

        /// @brief 设置位置（左上角）
        /// @param x 左上角 x 坐标
        /// @param y 左上角 y 坐标
        void setPosition(f32 x, f32 y);

        /// @brief 设置尺寸
        /// @param width 宽度
        /// @param height 高度
        void setSize(f32 width, f32 height);

        /// @brief 设置圆角半径
        /// @param radius 圆角半径（自动限制不超过宽/高的一半）
        void setCornerRadius(f32 radius);

        /// @brief 设置颜色
        /// @param color 填充颜色
        void setColor(const ColorRGBA &color);

        /// @brief 设置圆角分段数
        /// @param segments 每个圆角的分段数，越大越平滑
        void setCornerSegments(i32 segments);

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
        f32 m_x;              ///< 左上角 x 坐标
        f32 m_y;              ///< 左上角 y 坐标
        f32 m_width;          ///< 宽度
        f32 m_height;         ///< 高度
        f32 m_cornerRadius;   ///< 圆角半径
        i32 m_cornerSegments; ///< 每个圆角的分段数
        ColorRGBA m_color;    ///< 当前颜色

        std::vector<Vertex> m_vertices; ///< 顶点数组：[0]=中心点，[1..n]=轮廓点

        /// @brief 重新计算所有顶点位置
        void updateVertices();
    };
}