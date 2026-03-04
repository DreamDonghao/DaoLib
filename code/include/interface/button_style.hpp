#pragma once
#include <interface/button.hpp>
#include "drawable.hpp"

namespace dao {
    /// @brief 按钮样式接口
    class ButtonStyle : Drawable {
    public:
        ~ButtonStyle() override = default;

        /// @brief 加入绘制批处理
        void writeToBatch(BatchRenderer &builder) const override = 0;

        /// @brief 绑定按钮
        virtual void bindButton(Button *button) = 0;
    };
}
