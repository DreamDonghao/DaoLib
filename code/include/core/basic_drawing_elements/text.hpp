//
// Created by donghao on 25-12-18.
//
#ifndef TEXT_HPP
#define TEXT_HPP
#include <SDL3/SDL_pixels.h>
#include "core/tool/better_stl.hpp"
#include "color_rgb.hpp"

namespace dao {
    /// @brief 文本
    class Text {
    public:
        Text() = default;

        Text(const float32 x, const float32 y, std::u32string content, const float32 fontSize, const ColorRGBA color)
            : m_x(x), m_y(y), m_content(std::move(content)), m_fontSize(fontSize), m_color(color) {
        }

        void setX(const float32 x) { m_x = x; }

        void setY(const float32 y) { m_y = y; }

        void setContent(std::u32string content) { m_content = std::move(content); }

        void setFontSize(const float32 fontSize) { m_fontSize = fontSize; }

        void setColor(const ColorRGBA &color) { m_color = color; }

        void insert(const uint64 index, const char32_t ch) {
            m_content.insert(index, 1, ch);
        }

        [[nodiscard]] float32 getX() const { return m_x; }

        [[nodiscard]] float32 getY() const { return m_y; }

        [[nodiscard]] std::u32string getContent() const { return m_content; }

        [[nodiscard]] float32 getFontSize() const { return m_fontSize; }

        [[nodiscard]] SDL_FColor getColor() const {
            return {rgbNorm[m_color.r], rgbNorm[m_color.g], rgbNorm[m_color.b], m_color.a};
        }

    private:
        float m_x{0.0f}, m_y{0.0f};
        std::u32string m_content;
        float32 m_fontSize{32.0f};
        ColorRGBA m_color{255, 255, 255, 1.0f};
    };
}
#endif //TEXT_HPP
