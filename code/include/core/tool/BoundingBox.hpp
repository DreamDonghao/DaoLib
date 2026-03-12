#pragma once
#include <core/tool/type.hpp>
namespace dao {
    ///@brief 边界框
    ///@details 用来表示一个矩形范围区域左上右下边界的坐标
    class BoundingBox {
    public:
        BoundingBox() = default;

        /// @brief 构造函数
        /// @param left 左边界坐标
        /// @param top 上边界坐标
        /// @param right 右边界坐标
        /// @param bottom 下边界坐标
        BoundingBox(f32 left, f32 top, f32 right, f32 bottom);

        /// @brief 设置数据
        /// @param left 左边界坐标
        /// @param top 上边界坐标
        /// @param right 右边界坐标
        /// @param bottom 下边界坐标
        void set(f32 left, f32 top, f32 right, f32 bottom);

        /// @brief 设置左边界坐标
        /// @param left 左边界坐标
        void setLeft(const f32 left) { m_left = left; }

        /// @brief 设置上边界坐标
        /// @param top 上边界坐标
        void setTop(const f32 top) { m_top = top; }

        /// @brief 设置右边界坐标
        /// @param right 右边界坐标
        void setRight(const f32 right) { m_right = right; }

        /// @brief 设置下边界坐标
        /// @param bottom 下边界坐标
        void setBottom(const f32 bottom) { m_bottom = bottom; }

        /// @brief 水平移动
        /// @details 正右负左
        void moveHorizontal(f32 distance);

        /// @brief 竖直移动
        /// @details 正上负下
        void moveVertical(f32 distance);

        /// @brief 正常化
        /// @details 使颠倒的数据正常，若正常则无效果
        void normalize();

        /// @brief 检查是否有效
        /// @returns 有效返回 true 否则 false
        [[nodiscard]] bool isValid() const;

        /// @brief 判断点 (x,y) 是否在边界框内
        /// @returns 点 (x,y) 在边界框 返回 true 否则 false
        [[nodiscard]] bool isInBoundingBox(f32 x, f32 y) const;

        /// @brief 判断两个边界框是否有重合部分
        /// @returns 有重合部分返回 true 否则 false
        [[nodiscard]] bool isIntersects(const BoundingBox &other) const;

        /// @brief 获取左边界坐标
        /// @return 左边界坐标
        [[nodiscard]] f32 getLeft() const { return m_left; }

        /// @brief 获取上边界坐标
        /// @return 上边界坐标
        [[nodiscard]] f32 getTop() const { return m_top; }

        /// @brief 获取右边界坐标
        /// @return 右边界坐标
        [[nodiscard]] f32 getRight() const { return m_right; }

        /// @brief 获取下边界坐标
        /// @return 下边界坐标
        [[nodiscard]] f32 getBottom() const { return m_bottom; }

        /// @brief 获取宽度
        /// @returns 宽度值
        [[nodiscard]] f32 getWidth() const { return m_right - m_left; }

        /// @brief 获取高度
        /// @returns 高度值
        [[nodiscard]] f32 getHeight() const { return m_bottom - m_top; }

    private:
        f32 m_left = 0.f;
        f32 m_top = 0.f;
        f32 m_right = 0.f;
        f32 m_bottom = 0.f;
    };
}