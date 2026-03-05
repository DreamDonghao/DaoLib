#pragma once
#include <interface/IButtonStyle.hpp>

namespace dao {
    class SimpleTextButtonStyle : public ifc::IButtonStyle {
    public:
        SimpleTextButtonStyle()
            : SimpleTextButtonStyle(0, 0, 0, 0, U"", 0, ColorRGBA{}, nullptr) {
        }

        SimpleTextButtonStyle(const f32 x, const f32 y, const f32 w, const f32 h,
                              const std::u32string &text, const f32 textSize, const ColorRGBA textColor,
                              ifc::IButton *button)
            : m_text(x, y, textSize, textColor,text), m_button(button) {
        }

        SimpleTextButtonStyle(const SimpleTextButtonStyle &other) noexcept {
            m_text = other.m_text;
            m_button = other.m_button;
        }

        SimpleTextButtonStyle(SimpleTextButtonStyle &&other) noexcept {
            m_text = std::move(other.m_text);
            m_button = other.m_button;
        }

        void writeToBatch(BatchRenderer &builder) const override {
            if (m_button->isEnable()) {
                builder.addToBatch(m_text);
            }
        }

        void setText(const std::u32string &text) {
            m_text.setContent(text);
        }

        void bindButton(ifc::IButton *button) override {
            m_button = button;
        }

    private:
        Text m_text;
        ifc::IButton *m_button;
    };
}

