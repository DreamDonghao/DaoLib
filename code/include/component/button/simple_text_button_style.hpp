//
// Created by donghao on 25-12-18.
//
#ifndef SIMPLE_TEXT_BUTTON_STYLE_HPP
#define SIMPLE_TEXT_BUTTON_STYLE_HPP
#include "interface/button_style.hpp"

namespace dao {
    class SimpleTextButtonStyle : public ButtonStyle {
    public:
        SimpleTextButtonStyle()
            : SimpleTextButtonStyle(0, 0, 0, 0, U"", 0, ColorRGBA{}, nullptr) {
        }

        SimpleTextButtonStyle(const f32 x, const f32 y, const f32 w, const f32 h,
                              const std::u32string &text, const f32 textSize, const ColorRGBA textColor,
                              Button *button)
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

        void bindButton(Button *button) override {
            m_button = button;
        }

    private:
        Text m_text;
        Button *m_button;
    };
}
#endif //SIMPLE_TEXT_BUTTON_STYLE_HPP
