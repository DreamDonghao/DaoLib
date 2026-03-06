#pragma once
#include <component/graph/Circle.hpp>

namespace dao {
    /// @brief 实心圆形图形
    /// @details 单一颜色的实心圆形，内部使用 Circle 实现
    class SolidCircle : public ifc::IGraph {
    public:
        /// @brief 构造函数
        /// @param cx 圆心 x 坐标
        /// @param cy 圆心 y 坐标
        /// @param radius 半径
        /// @param color 填充颜色
        /// @param segments 圆形分段数（默认32）
        SolidCircle(const f32 cx, const f32 cy, const f32 radius,
            const ColorRGBA &color, const i32 segments = 32)
            : m_circle(cx, cy, radius, color, color, segments) {
        }

        /// @brief 将实心圆形写入批处理渲染器
        /// @param batchRenderer 批处理渲染器引用
        void writeToBatch(BatchRenderer &batchRenderer) const override {
            m_circle.writeToBatch(batchRenderer);
        }

        /// @brief 旋转实心圆形
        /// @param cx 旋转中心 x 坐标
        /// @param cy 旋转中心 y 坐标
        /// @param theta 旋转角度（弧度）
        void rotate(const f32 cx, const f32 cy, const f32 theta) override {
            m_circle.rotate(cx, cy, theta);
        }

        /// @brief 平移实心圆形
        /// @param dx x 方向平移量
        /// @param dy y 方向平移量
        void translate(const f32 dx, const f32 dy) override {
            m_circle.translate(dx, dy);
        }

    private:
        Circle m_circle; ///< 内部圆形
    };
}