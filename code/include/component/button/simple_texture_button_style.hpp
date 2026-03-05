#pragma once
#include "simple_button.hpp"
#include "interface/IButtonStyle.hpp"

namespace dao {

    class SimpleTextureButtonStyle : public ifc::IButtonStyle {
    public:
        SimpleTextureButtonStyle(
            const f32 x, const f32 y, const f32 w, const f32 h,
            const i32 atnId, const i32 athId, const i32 atpId,
            ifc::IButton *button)
            : m_normalStyle(atnId, x, y, x + w, y + h), m_hoverStyle(athId, x, y, x + w, y + h),
              m_pressedStyle(atpId, x, y, x + w, y + h), m_button(button) {
        }

        SimpleTextureButtonStyle(SimpleButton &button, const i32 atnId, const i32 athId, const i32 atpId)
            : m_normalStyle(atnId, button.getBoundingBox()), m_hoverStyle(athId, button.getBoundingBox()),
              m_pressedStyle(atpId, button.getBoundingBox()), m_button(&button) {
        }

        void bindButton(ifc::IButton *button) override {
            m_button = button;
        }

        void writeToBatch(BatchRenderer &builder) const override {
            if (m_button->isEnable()) {
                if (m_button->getStatus() == ButtonStatus::Normal) {
                    builder.addToBatch(m_normalStyle);
                } else if (m_button->getStatus() == ButtonStatus::Hover) {
                    builder.addToBatch(m_hoverStyle);
                } else if (m_button->getStatus() == ButtonStatus::Pressed) {
                    builder.addToBatch(m_pressedStyle);
                }
            }
        }

    private:
        Text m_text;
        AtlasTexture m_normalStyle;
        AtlasTexture m_hoverStyle;
        AtlasTexture m_pressedStyle;
        ifc::IButton *m_button{nullptr};
    };
}
