#pragma once
#include <component/graph/Rect.hpp>

namespace dao {
    /// @brief 实心矩形图形
    /// @details 单一颜色的实心矩形，内部使用 Rect 实现
    class SolidRect : public ifc::IGraph {
    public:
        /// @brief 构造函数
        /// @param x 矩形左上角 x 坐标
        /// @param y 矩形左上角 y 坐标
        /// @param width 矩形宽度
        /// @param height 矩形高度
        /// @param color 填充颜色
        SolidRect(const f32 x, const f32 y, const f32 width, const f32 height, const ColorRGBA &color)
            : m_rect(x, y, width, height, color, color, color, color) {
        }

        /// @brief 将实心矩形写入批处理渲染器
        /// @param batchRenderer 批处理渲染器引用
        void writeToBatch(BatchRenderer &batchRenderer) const override {
            m_rect.writeToBatch(batchRenderer);
        }

        /// @brief 旋转实心矩形
        /// @param cx 旋转中心 x 坐标
        /// @param cy 旋转中心 y 坐标
        /// @param theta 旋转角度（弧度）
        void rotate(const f32 cx, const f32 cy, const f32 theta) override {
            m_rect.rotate(cx, cy, theta);
        }

        /// @brief 平移实心矩形
        /// @param dx x 方向平移量
        /// @param dy y 方向平移量
        void translate(const f32 dx, const f32 dy) override {
            m_rect.translate(dx, dy);
        }

    private:
        Rect m_rect; ///< 内部矩形
    };
}