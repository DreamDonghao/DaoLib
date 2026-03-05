#pragma once
#include <SDL3/SDL_pixels.h>
#include "core/tool/better_stl.hpp"
#include "primitives/ColorRGB.hpp"

namespace dao {
    /// @brief 文本渲染对象
    /// @details 表示一个可渲染的文本对象，包含位置、字体大小、颜色和UTF-32编码的内容
    /// 支持通过批处理系统渲染，可动态修改文本属性和内容
    class Text {
    public:
        Text() = default;

        /// @brief 构造函数
        /// @param x 文本左上角的x坐标
        /// @param y 文本左上角的y坐标
        /// @param fontSize 字体大小（像素高度），默认32.0f
        /// @param color 文本颜色，默认黑色
        /// @param content UTF-32编码的文本内容，默认为空字符串
        Text(const f32 x, const f32 y, const f32 fontSize, const ColorRGBA color = White,
             utf32str content = U"")
            : m_x(x), m_y(y), m_fontSize(fontSize), m_color(color), m_content(std::move(content)) {
        }

        /// @brief 在文本指定位置插入一个字符
        /// @param index 插入位置索引（0为开头）
        /// @param ch 要插入的UTF-32字符
        void insert(const i64 index, const utf32char ch) {
            m_content.insert(index, 1, ch);
        }

        /// @brief 设置文本x坐标
        void setX(const f32 x) { m_x = x; }

        /// @brief 设置文本y坐标
        void setY(const f32 y) { m_y = y; }

        /// @brief 设置文本内容
        /// @param content UTF-32编码的新文本内容
        void setContent(std::u32string content) { m_content = std::move(content); }

        /// @brief 设置字体大小
        /// @param fontSize 新的字体大小（像素高度）
        void setFontSize(const f32 fontSize) { m_fontSize = fontSize; }

        /// @brief 设置文本颜色
        /// @param color 新的文本颜色
        void setColor(const ColorRGBA &color) { m_color = color; }

        /// @brief 获取文本x坐标
        /// @return 文本左上角的x坐标
        [[nodiscard]] f32 getX() const { return m_x; }

        /// @brief 获取文本y坐标
        /// @return 文本左上角的y坐标
        [[nodiscard]] f32 getY() const { return m_y; }

        /// @brief 获取文本内容
        /// @return UTF-32编码的文本内容的常量引用
        [[nodiscard]] const utf32str &getContent() const { return m_content; }

        /// @brief 获取字体大小
        /// @return 字体大小（像素高度）
        [[nodiscard]] f32 getFontSize() const { return m_fontSize; }

        /// @brief 获取SDL格式的文本颜色
        /// @details 将ColorRGBA转换为SDL_FColor格式，用于SDL渲染
        /// @return SDL_FColor格式的文本颜色
        [[nodiscard]] SDL_FColor getColor() const {
            return {rgbNorm[m_color.r()], rgbNorm[m_color.g()], rgbNorm[m_color.b()], m_color.a()};
        }

    private:
        f32 m_x{0.0f};         ///< 文本左上角的x坐标
        f32 m_y{0.0f};         ///< 文本左上角的y坐标
        f32 m_fontSize{32.0f}; ///< 字体大小（像素高度），默认32.0f
        ColorRGBA m_color{};   ///< 文本颜色，默认黑色
        utf32str m_content{};  ///< UTF-32编码的文本内容
    };
}
