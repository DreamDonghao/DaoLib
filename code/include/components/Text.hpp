#pragma once
#include <core/tool/type.hpp>
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/ColorRGB.hpp>

namespace dao {
    /// @brief 文本渲染组件
    /// @details 基础渲染元素，负责在指定位置渲染文本
    /// 不处理区域限制和自动换行，如需这些功能请使用 TextBox
    class Text {
    public:
        /// @brief 构造函数
        /// @param x 起始 x 坐标
        /// @param y 起始 y 坐标
        /// @param lineHeight 行高
        /// @param color 文本颜色
        /// @param str 文本内容（UTF-32）
        explicit Text(const f32 x, const f32 y, const f32 lineHeight,
                      const ColorRGBA &color = Black, utf32str str = U"")
            : m_x(x), m_y(y), m_lineHeight(lineHeight), m_color(color.getSDLFColor()),
              m_str(std::move(str)) {
        }

        /// @brief 写入批处理渲染器
        /// @param batchRenderer 批处理渲染器
        void writeToBatch(BatchRenderer &batchRenderer) const {
            const auto &atlas = batchRenderer.getGlyphAtlas();
            auto vertices = batchRenderer.allocateVertices(
                1,
                6 * static_cast<i32>(std::ranges::ssize(m_str))
            );

            f32 x = m_x;
            f32 y = m_y;

            for (const auto ch: m_str) {
                if (ch == U'\n') {
                    y += m_lineHeight;
                    x = m_x;
                    continue;
                }

                batchRenderer.loadGlyph(ch);
                const auto region = atlas.getGlyphAtlasRegion(ch);
                const f32 width = m_lineHeight / region.getHeight() * region.getWidth();

                vertices[0] = {{x, y}, m_color, {region.getLeft(), region.getTop()}};
                vertices[1] = {{x + width, y}, m_color, {region.getRight(), region.getTop()}};
                vertices[2] = {{x + width, y + m_lineHeight}, m_color, {region.getRight(), region.getBottom()}};
                vertices[3] = vertices[2];
                vertices[4] = {{x, y + m_lineHeight}, m_color, {region.getLeft(), region.getBottom()}};
                vertices[5] = vertices[0];
                vertices += 6;

                x += width;
            }
        }

        /// @brief 设置文本内容
        void setContent(utf32str str) { m_str = std::move(str); }

        /// @brief 获取文本内容
        [[nodiscard]] const utf32str &getContent() const { return m_str; }

        /// @brief 设置位置
        void setPosition(const f32 x, const f32 y) { m_x = x; m_y = y; }

        /// @brief 获取 x 坐标
        [[nodiscard]] f32 x() const { return m_x; }

        /// @brief 获取 y 坐标
        [[nodiscard]] f32 y() const { return m_y; }

        /// @brief 设置行高
        void setLineHeight(const f32 lineHeight) { m_lineHeight = lineHeight; }

        /// @brief 获取行高
        [[nodiscard]] f32 lineHeight() const { return m_lineHeight; }

        /// @brief 设置颜色
        void setColor(const ColorRGBA &color) { m_color = color.getSDLFColor(); }

    private:
        f32 m_x;
        f32 m_y;
        f32 m_lineHeight;
        SDL_FColor m_color;
        utf32str m_str;
    };
}
