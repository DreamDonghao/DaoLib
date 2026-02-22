//
// Created by donghao on 25-11-27.
//
#ifndef BOUNDING_BOX_HPP
#define BOUNDING_BOX_HPP
#include "better_stl.hpp"

namespace dao {
    ///@brief 边界框
    ///@details 用来表示一个矩形范围区域左上右下边界的坐标
    class BoundingBox {
    public:
        BoundingBox() = default;

        /// @brief 构造函数
        /// @param left 左边界坐标
        /// @param up 上边界坐标
        /// @param right 右边界坐标
        /// @param down 下边界坐标
        BoundingBox(f32 left, f32 up, f32 right, f32 down);

        ~BoundingBox() = default;

        /// @brief 设置数据
        /// @param left 左边界坐标
        /// @param up 上边界坐标
        /// @param right 右边界坐标
        /// @param  down 下边界坐标
        void set(float left, float up, float right, float down);

        /// @brief 设置左边界坐标
        /// @param left 左边界坐标
        void setLeft(const float left) { m_left = left; }

        /// @brief 设置上边界坐标
        /// @param up 上边界坐标
        void setUp(const float up) { m_up = up; }

        /// @brief 设置右边界坐标
        /// @param right 右边界坐标
        void setRight(const float right) { m_right = right; }

        /// @brief 设置下边界坐标
        /// @param down 下边界坐标
        void setDown(const float down) { m_down = down; }

        /// @brief 水平移动
        /// @details 正右负左
        void moveHorizontal(float distance);

        /// @brief 竖直移动
        /// @details 正上负下
        void moveVertical(float distance);

        /// @brief 正常化
        /// @details 使颠倒的数据正常，若正常则无效果
        void normalize();

        /// @brief 检查是否有效
        /// @returns 有效返回 ture 否则 false
        [[nodiscard]] bool isValid() const;

        /// @brief 判断点 (x,y) 是否在边界框内
        /// @returns 点 (x,y) 在边界框 返回 true 否则 false
        [[nodiscard]] bool isInBoundingBox(float x, float y) const;

        /// @brief 判断两个边界框是否有重合部分
        /// @returns 有重合部分返回 true 否者false
        [[nodiscard]] bool isIntersects(const BoundingBox &other) const;


        /// @brief 获取左边界坐标
        /// @return 左边界坐标
        [[nodiscard]] float getLeft() const { return m_left; }

        /// @brief 获取上边界坐标
        /// @return 上边界坐标
        [[nodiscard]] float getTop() const { return m_up; }

        /// @brief 获取右边界坐标
        /// @return 右边界x坐标值
        [[nodiscard]] float getRight() const { return m_right; }

        /// @brief 获取下边界坐标
        /// @return 下边界坐标值
        [[nodiscard]] float getBottom() const { return m_down; }

        /// @brief 获取宽度
        /// @returns 宽度值
        [[nodiscard]] float getWidth() const { return m_right - m_left; }

        /// @brief 获取高度
        /// @returns 高度值
        [[nodiscard]] float getHeight() const { return m_down - m_up; }

    private:
        float m_left = 0.f;
        float m_up = 0.f;
        float m_right = 0.f;
        float m_down = 0.f;
    };
}
#endif //BOUNDING_BOX_HPP
