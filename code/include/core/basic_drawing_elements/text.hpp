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

        /// @param x 横坐标
        /// @param y 纵坐标
        /// @param content 内容
        /// @param fontSize 字体大小(高度)
        /// @param color 字体颜色
        Text(const f32 x, const f32 y, const f32 fontSize, const ColorRGBA color = {0, 0, 0, 1},
             utf32str content = U"")
            : m_x(x), m_y(y), m_fontSize(fontSize), m_color(color), m_content(std::move(content)) {
        }

        void insert(const u64 index, const utf32char ch) {
            m_content.insert(index, 1, ch);
        }

        void setX(const f32 x) { m_x = x; }
        void setY(const f32 y) { m_y = y; }
        void setContent(std::u32string content) { m_content = std::move(content); }
        void setFontSize(const f32 fontSize) { m_fontSize = fontSize; }
        void setColor(const ColorRGBA &color) { m_color = color; }
        [[nodiscard]] f32 getX() const { return m_x; }
        [[nodiscard]] f32 getY() const { return m_y; }
        [[nodiscard]] const utf32str &getContent() const { return m_content; }
        [[nodiscard]] f32 getFontSize() const { return m_fontSize; }

        [[nodiscard]] SDL_FColor getColor() const {
            return {rgbNorm[m_color.r], rgbNorm[m_color.g], rgbNorm[m_color.b], m_color.a};
        }

    private:
        f32 m_x{0.0f};
        f32 m_y{0.0f};
        f32 m_fontSize{32.0f};
        ColorRGBA m_color{255, 255, 255, 1.0f};
        utf32str m_content{};
    };
}
#endif //TEXT_HPP
