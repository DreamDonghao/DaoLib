
#pragma once
#include <functional>
#include <SDL3/SDL_events.h>
#include <component/graph/Rect.hpp>
#include <interface/IButton.hpp>

namespace dao {
    /// @brief 简单按钮组件
    /// @details 实现了基本的按钮功能，包括响应鼠标事件和触发点击回调
    class SimpleButton : public ifc::IButton {
    public:
        /// @brief 默认析构函数
        ~SimpleButton() override = default;

        /// @brief 默认构造函数
        /// @details 创建一个空按钮（0x0大小，位于原点）
        SimpleButton()
            : SimpleButton(0.0f, 0.0f, 0.0f, 0.0f, [] {
            }) {
        }

        /// @brief 构造函数
        /// @param x 按钮左上角 x 坐标
        /// @param y 按钮左上角 y 坐标
        /// @param width 按钮宽度
        /// @param height 按钮高度
        /// @param onClick 点击回调函数
        /// @param buttonStatus 按钮初始状态，默认为Normal
        SimpleButton(f32 x, f32 y, f32 width, f32 height,
                     std::move_only_function<void()> onClick,
                     ButtonStatus buttonStatus = ButtonStatus::Normal);

        /// @brief 禁止拷贝构造
        SimpleButton(const SimpleButton &) = delete;

        /// @brief 禁止拷贝赋值
        SimpleButton &operator=(const SimpleButton &) = delete;

        /// @brief 移动构造函数
        /// @param other 要移动的按钮对象
        SimpleButton(SimpleButton &&other) noexcept {
            m_boundingBox = other.m_boundingBox;
            m_status = other.m_status;
            m_onClick = std::move(other.m_onClick);
        }

        /// @brief 移动赋值运算符
        /// @param other 要移动的按钮对象
        /// @return 当前按钮对象的引用
        SimpleButton &operator=(SimpleButton &&other) noexcept {
            m_boundingBox = other.m_boundingBox;
            m_status = other.m_status;
            m_onClick = std::move(other.m_onClick);
            return *this;
        }

        /// @brief 处理鼠标事件
        /// @param event SDL 事件对象，处理鼠标移动、按下和释放事件
        void handleEvent(const SDL_Event &event) override;

        /// @brief 检查按钮是否启用
        /// @return 如果按钮未被禁用则返回true，否则返回false
        [[nodiscard]] bool isEnable() const override {
            return m_status != ButtonStatus::Disabled;
        }

        /// @brief 获取当前状态
        /// @return 按钮当前的状态
        [[nodiscard]] ButtonStatus getStatus() const override {
            return m_status;
        }

        /// @brief 设置状态
        /// @param status 新的按钮状态
        void setStatus(const ButtonStatus status) {
            m_status = status;
        }

        /// @brief 获取边界框
        /// @return 按钮的边界框
        [[nodiscard]] BoundingBox getBoundingBox() const {
            return m_boundingBox;
        }

    private:
        BoundingBox m_boundingBox{};      ///< 按钮边界框
        ButtonStatus m_status{ButtonStatus::Disabled};  ///< 按钮当前状态
        std::move_only_function<void()> m_onClick;      ///< 点击回调函数
    };
}
