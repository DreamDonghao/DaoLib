#pragma once
#include <functional>
#include <SDL3/SDL_events.h>
#include <components/controls/IButton.hpp>

#include "components/graphs/Rectangle.hpp"
#include "core/render/BatchRenderer.hpp"
#include "core/tool/BoundingBox.hpp"
#include "core/tool/type.hpp"

namespace dao {
    /// @brief 简单按钮组件
    /// @details 实现了基本的按钮功能，包括响应鼠标事件和触发点击回调
    class Button : public ifc::IButton {
    public:
        /// @brief 默认析构函数
        ~Button() override = default;

        /// @brief 默认构造函数
        /// @details 创建一个空按钮（0x0大小，位于原点）
        Button()
            : Button(0.0f, 0.0f, 0.0f, 0.0f, [] {
            }) {
        }

        /// @brief 构造函数
        /// @param x 按钮左上角 x 坐标
        /// @param y 按钮左上角 y 坐标
        /// @param width 按钮宽度
        /// @param height 按钮高度
        /// @param onClick 点击回调函数
        /// @param buttonStatus 按钮初始状态，默认为Normal
        Button(f32 x, f32 y, f32 width, f32 height,
               std::move_only_function<void()> onClick,
               ButtonStatus buttonStatus = ButtonStatus::Normal)
            : m_boundingBox(x, y, x + width, y + height),
              m_status(buttonStatus), m_onClick(std::move(onClick)), m_rectangle(x, y, width, height,Green) {
        }

        /// @brief 禁止拷贝构造
        Button(const Button &) = delete;

        /// @brief 禁止拷贝赋值
        Button &operator=(const Button &) = delete;

        /// @brief 移动构造函数
        /// @param other 要移动的按钮对象
        Button(Button &&other) noexcept : m_rectangle() {
            m_boundingBox = other.m_boundingBox;
            m_status = other.m_status;
            m_onClick = std::move(other.m_onClick);
            m_rectangle = other.m_rectangle;
        }

        /// @brief 移动赋值运算符
        /// @param other 要移动的按钮对象
        /// @return 当前按钮对象的引用
        Button &operator=(Button &&other) noexcept {
            m_boundingBox = other.m_boundingBox;
            m_status = other.m_status;
            m_onClick = std::move(other.m_onClick);
            return *this;
        }

        /// @brief 处理鼠标事件
        /// @param event SDL 事件对象，处理鼠标移动、按下和释放事件
        void handleEvent(const SDL_Event &event) override {
            if (m_status == ButtonStatus::Disabled) {
                return;
            }
            float x = 0, y = 0;
            if (event.type == SDL_EVENT_MOUSE_MOTION) {
                x = event.motion.x;
                y = event.motion.y;
            } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
                       event.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                x = event.button.x;
                y = event.button.y;
            } else {
                return;
            }
            if (!m_boundingBox.isInBoundingBox(x, y)) {
                m_status = ButtonStatus::Normal;
                return;
            }
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN &&
                event.button.button == SDL_BUTTON_LEFT) {
                m_status = ButtonStatus::Pressed;
                return;
            }
            if (event.type == SDL_EVENT_MOUSE_BUTTON_UP &&
                event.button.button == SDL_BUTTON_LEFT &&
                m_status == ButtonStatus::Pressed) {
                m_status = ButtonStatus::Hover;
                m_onClick();
                return;
            }
            m_status = ButtonStatus::Hover;
        }

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

        /// @brief 设置位置
        void setPosition(const f32 x, const f32 y) {
            const f32 w = m_boundingBox.getWidth();
            const f32 h = m_boundingBox.getHeight();
            m_boundingBox = BoundingBox(x, y, x + w, y + h);
            m_rectangle.setPosition(x, y);
        }

        void writeToBatch(BatchRenderer &batchRenderer) const {
            m_rectangle.writeToBatch(batchRenderer);
        }

    private:
        BoundingBox m_boundingBox{};                   ///< 按钮边界框
        ButtonStatus m_status{ButtonStatus::Disabled}; ///< 按钮当前状态
        std::move_only_function<void()> m_onClick;     ///< 点击回调函数

        mutable Rectangle m_rectangle;
    };
}
