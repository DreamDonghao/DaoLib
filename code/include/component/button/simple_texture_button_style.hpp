//
// Created by donghao on 25-12-12.
//
#ifndef SIMPLE_BUTTON_STYLE_HPP
#define SIMPLE_BUTTON_STYLE_HPP
#include "simple_button.hpp"
#include "interface/button_style.hpp"

namespace dao {

    class SimpleTextureButtonStyle : public ButtonStyle {
    public:
        SimpleTextureButtonStyle(
            const float32 x, const float32 y, const float32 w, const float32 h,
            const uint32 atnId, const uint32 athId, const uint32 atpId,
            Button *button)
            : m_normalStyle(atnId, x, y, x + w, y + h), m_hoverStyle(athId, x, y, x + w, y + h),
              m_pressedStyle(atpId, x, y, x + w, y + h), m_button(button) {
        }

        SimpleTextureButtonStyle(SimpleButton &button, const uint32 atnId, const uint32 athId, const uint32 atpId)
            : m_normalStyle(atnId, button.getBoundingBox()), m_hoverStyle(athId, button.getBoundingBox()),
              m_pressedStyle(atpId, button.getBoundingBox()), m_button(&button) {
        }

        void bindButton(Button *button) override {
            m_button = button;
        }

        void writeToBatch(VertexBatchBuilder &builder) const override {
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
        Button *m_button{nullptr};
    };
}
#endif //SIMPLE_BUTTON_STYLE_HPP
