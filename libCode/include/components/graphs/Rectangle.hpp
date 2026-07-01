#pragma once
#include <core/tool/type.hpp>
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/Vertex.hpp>
#include <components/graphs/IGraph.hpp>

namespace dao {
    /// @brief 矩形图形组件
    /// @details 轴对齐矩形，由左上角位置、宽度和高度定义，以 2 个三角形方式渲染
    class Rectangle : public ifc::IGraph {
    public:
        /// @brief 构造函数
        /// @param x 左上角 x 坐标
        /// @param y 左上角 y 坐标
        /// @param width 宽度
        /// @param height 高度
        /// @param color 填充颜色，默认为白色
        explicit Rectangle(f32 x = 0.0f, f32 y = 0.0f, f32 width = 0.0f, f32 height = 0.0f,
                           const ColorRGBA &color = White);

        /// @brief 设置位置（左上角）
        /// @param x 左上角 x 坐标
        /// @param y 左上角 y 坐标
        void setPosition(f32 x, f32 y);

        /// @brief 设置尺寸
        /// @param width 宽度
        /// @param height 高度
        void setSize(f32 width, f32 height);

        /// @brief 设置颜色
        /// @param color 填充颜色
        void setColor(const ColorRGBA &color);

        /// @brief 获取左上角 x 坐标
        [[nodiscard]] f32 x() const noexcept { return m_x; }

        /// @brief 获取左上角 y 坐标
        [[nodiscard]] f32 y() const noexcept { return m_y; }

        /// @brief 获取宽度
        [[nodiscard]] f32 width() const noexcept { return m_width; }

        /// @brief 获取高度
        [[nodiscard]] f32 height() const noexcept { return m_height; }

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
        f32 m_x;      ///< 左上角 x 坐标
        f32 m_y;      ///< 左上角 y 坐标
        f32 m_width;  ///< 宽度
        f32 m_height; ///< 高度

        std::vector<Vertex> m_vertices; ///< 顶点数组：[0]=左上，[1]=右上，[2]=左下，[3]=右下

        /// @brief 重新计算所有顶点位置
        /// @param color 顶点颜色
        void updateVertices(const ColorRGBA &color);
    };
}