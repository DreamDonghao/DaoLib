#pragma once
#include <core/tool/type.hpp>
#include <utility>
#include <numbers>

namespace dao {
    /// @brief 圆周率常量（单精度浮点数）
    inline constexpr f32 f32pi = std::numbers::pi_v<float>;

    /// @brief 二维点类，表示平面上的一个点坐标
    class Point {
    public:
        /// @brief 默认构造函数
        Point() = default;

        /// @brief 使用指定坐标构造点对象
        /// @param x X 坐标
        /// @param y Y 坐标
        Point(const f32 x, const f32 y) : m_x(x), m_y(y) {
        }

        /// @brief 设置点的坐标
        /// @param x X 坐标
        /// @param y Y 坐标
        void setPoint(const f32 x, const f32 y) {
            m_x = x;
            m_y = y;
        }

        /// @brief 设置点的 X 坐标
        /// @param x X 坐标
        void setX(const f32 x) {
            m_x = x;
        }

        /// @brief 设置点的 Y 坐标
        /// @param y Y 坐标
        void setY(const f32 y) {
            m_y = y;
        }

        /// @brief 获取点的 X 坐标
        /// @return X 坐标
        [[nodiscard]] f32 x() const {
            return m_x;
        }

        /// @brief 获取点的 Y 坐标
        /// @return Y 坐标
        [[nodiscard]] f32 y() const {
            return m_y;
        }

    private:
        f32 m_x{0.0f}; ///< X 坐标
        f32 m_y{0.0f}; ///< Y 坐标
    };

    /// @brief 旋转向量，将点绕指定中心点旋转指定角度
    /// @param px 待旋转点的 X 坐标
    /// @param py 待旋转点的 Y 坐标
    /// @param cx 旋转中心的 X 坐标
    /// @param cy 旋转中心的 Y 坐标
    /// @param theta 旋转角度（弧度），在屏幕坐标系中为顺时针旋转，数学坐标系中为逆时针旋转
    /// @return 旋转后的点坐标 (x, y)
    inline std::pair<f32, f32> rotatePointRad(
        const f32 px, const f32 py,
        const f32 cx, const f32 cy, const f32 theta) {
        const float cosA = std::cos(theta);
        const float sinA = std::sin(theta);

        const float dx = px - cx;
        const float dy = py - cy;

        float rx = dx * cosA - dy * sinA;
        float ry = dx * sinA + dy * cosA;

        rx += cx;
        ry += cy;

        return {rx, ry};
    }
}
