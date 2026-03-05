#pragma once
#include <interface/IButton.hpp>
#include <interface/IDrawable.hpp>

namespace dao::ifc {
    /// @brief 按钮样式接口
    class IButtonStyle : IDrawable {
    public:
        ~IButtonStyle() override = default;

        /// @brief 写入批处理
        void writeToBatch(BatchRenderer &builder) const override = 0;

        /// @brief 绑定按钮
        virtual void bindButton(IButton *button) = 0;
    };
}
