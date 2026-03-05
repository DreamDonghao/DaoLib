
#pragma once
#include <functional>
#include <SDL3/SDL_events.h>
#include <component/graph/Rect.hpp>
#include <interface/IButton.hpp>

namespace dao {
    class SimpleButton : public ifc::IButton {
    public:
        ~SimpleButton() override = default;

        SimpleButton()
            : SimpleButton(0.0f, 0.0f, 0.0f, 0.0f, [] {
            }) {
        }

        SimpleButton(f32 x, f32 y, f32 width, f32 height,
                     std::move_only_function<void()> onClick,
                     ButtonStatus buttonStatus = ButtonStatus::Normal);

        SimpleButton(const SimpleButton &) = delete;

        SimpleButton &operator=(const SimpleButton &) = delete;

        SimpleButton(SimpleButton &&other) noexcept {
            m_boundingBox = other.m_boundingBox;
            m_status = other.m_status;
            m_onClick = std::move(other.m_onClick);
        }

        SimpleButton &operator=(SimpleButton &&other) noexcept {
            m_boundingBox = other.m_boundingBox;
            m_status = other.m_status;
            m_onClick = std::move(other.m_onClick);
            return *this;
        }

        /// @brief 处理事件
        void handleEvent(const SDL_Event &event) override;


        [[nodiscard]] bool isEnable() const override {
            return m_status != ButtonStatus::Disabled;
        }

        /// @brief 获取当前状态
        [[nodiscard]] ButtonStatus getStatus() const override {
            return m_status;
        }

        /// @brief 设置状态
        void setStatus(const ButtonStatus status) {
            m_status = status;
        }

        /// @brief 获取边界框
        [[nodiscard]] BoundingBox getBoundingBox() const {
            return m_boundingBox;
        }

    private:
        BoundingBox m_boundingBox{};
        ButtonStatus m_status{ButtonStatus::Disabled};
        std::move_only_function<void()> m_onClick;
    };
}
