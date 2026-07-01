#pragma once
union SDL_Event;

namespace dao {
    /// @brief 按钮状态
    enum class ButtonStatus {
        Disabled, ///< 禁用
        Normal,   ///< 自然
        Hover,    ///< 悬停
        Pressed,  ///< 按下
    };

    namespace ifc {
        /// @brief  按钮接口
        class IButton {
        public:
            virtual ~IButton() = default ;

            /// @brief 处理消息，更新状态
            virtual void handleEvent(const SDL_Event &event) = 0;

            /// @brief 获取当前状态
            [[nodiscard]] virtual ButtonStatus getStatus() const = 0;

            [[nodiscard]] virtual bool isEnable() const = 0;
        };
    }
}
