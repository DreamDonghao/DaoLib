#pragma once
#include <core/tool/type.hpp>
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/ColorRGB.hpp>
#include <core/render/IDrawable.hpp>

namespace dao {
    /// @brief 文本水平对齐
    enum class TextAlign { Left, Center, Right };

    /// @brief 文本渲染组件
    /// @details 单行/多行文本渲染，超出区域裁剪
    class Text : public ifc::IDrawable {
    public:
        /// @brief 单行构造（高度 = 行高）
        Text(const f32 x, const f32 y, const f32 w, const f32 lineHeight,
             const ColorRGBA &color = Black, utf32str str = U"")
            : m_x(x), m_y(y), m_w(w), m_h(lineHeight), m_lineHeight(lineHeight),
              m_color(color.getSDLFColor()), m_str(std::move(str)) {
        }

        /// @brief 多行构造（指定区域高度）
        Text(const f32 x, const f32 y, const f32 w, const f32 h, const f32 lineHeight,
             const ColorRGBA &color = Black, utf32str str = U"")
            : m_x(x), m_y(y), m_w(w), m_h(h), m_lineHeight(lineHeight),
              m_color(color.getSDLFColor()), m_str(std::move(str)) {
        }

        void writeToBatch(BatchRenderer &batchRenderer) const override {
            if (m_str.empty()) return;

            const auto &atlas = GlyphAtlas::getGlyphAtlas();
            const f32 fontScale = m_lineHeight / atlas.getFontSize();

            for (const auto ch: m_str) {
                if (ch != U'\n') batchRenderer.loadGlyph(ch);
            }

            // —— 布局：拆行 ——
            const i32 strLen = static_cast<i32>(m_str.size());
            struct Line { i32 begin, end; };
            std::vector<Line> layout;
            {
                i32 pos = 0;
                while (pos < strLen) {
                    const i32 lineBegin = pos;
                    i32 lastBreak = -1;
                    f32 lineW = 0;

                    while (pos < strLen) {
                        const char32_t ch = m_str[pos];
                        if (ch == U'\n') { ++pos; break; }

                        const f32 adv = atlas.getGlyphAdvance(ch) * fontScale;

                        if (m_wordWrap && lineW + adv > m_w && lineW > 0) {
                            if (lastBreak >= 0) pos = lastBreak;
                            break;
                        }

                        if (m_wordWrap && (ch == U' ' || ch == U'\t' || ch >= 0x2E80)) {
                            lastBreak = pos + 1;
                        }

                        lineW += adv;
                        ++pos;
                    }
                    layout.push_back({lineBegin, pos});
                }
            }

            // —— 计算总字符数 ——
            i32 totalChars = 0;
            for (const auto &line: layout) totalChars += line.end - line.begin;
            if (totalChars == 0) return;
            auto vertices = batchRenderer.allocateVertices(1, totalChars * 6);

            // —— 垂直对齐 ——
            const f32 totalH = static_cast<f32>(layout.size()) * m_lineHeight;
            f32 y = m_y;
            if (m_alignToEnd && totalH > m_h) {
                y = m_y + m_h - totalH;
            }

            const f32 clipLeft = m_x;
            const f32 clipRight = m_x + m_w;
            const f32 clipBottom = m_y + m_h;
            const f32 textOriginX = m_x - m_scrollX;

            for (const auto &line: layout) {
                if (y + m_lineHeight <= m_y) { y += m_lineHeight; continue; }
                if (y >= clipBottom) break;

                // 行宽
                f32 lineW = 0;
                for (i32 i = line.begin; i < line.end; ++i) {
                    lineW += atlas.getGlyphAdvance(m_str[i]) * fontScale;
                }

                // 水平对齐
                f32 x = textOriginX;
                switch (m_align) {
                    case TextAlign::Center: x -= lineW * 0.5f; break;
                    case TextAlign::Right:  x -= lineW; break;
                    default: break;
                }

                for (i32 i = line.begin; i < line.end; ++i) {
                    const char32_t ch = m_str[i];
                    const f32 adv = atlas.getGlyphAdvance(ch) * fontScale;
                    const auto region = atlas.getGlyphAtlasRegion(ch);
                    const f32 w = m_lineHeight / region.getHeight() * region.getWidth();

                    if (x + w < clipLeft) { x += adv; continue; }
                    if (x >= clipRight) break;

                    // 裁剪到 [clipLeft, clipRight]
                    f32 drawX = x, drawW = w;
                    f32 texL = region.getLeft(), texR = region.getRight();
                    if (x < clipLeft) {
                        const f32 r = (clipLeft - x) / w;
                        drawX = clipLeft;
                        drawW = w * (1.0f - r);
                        texL += (texR - texL) * r;
                    }
                    if (x + w > clipRight) {
                        const f32 r = (clipRight - x) / w;
                        drawW = w * r;
                        texR = texL + (texR - texL) * r;
                    }

                    vertices[0] = {{drawX, y}, m_color, {texL, region.getTop()}};
                    vertices[1] = {{drawX + drawW, y}, m_color, {texR, region.getTop()}};
                    vertices[2] = {{drawX + drawW, y + m_lineHeight}, m_color, {texR, region.getBottom()}};
                    vertices[3] = vertices[2];
                    vertices[4] = {{drawX, y + m_lineHeight}, m_color, {texL, region.getBottom()}};
                    vertices[5] = vertices[0];
                    vertices += 6;

                    x += adv;
                }

                y += m_lineHeight;
            }
        }

        // === 内容 ===
        void setContent(utf32str str) { m_str = std::move(str); }
        [[nodiscard]] const utf32str &getContent() const { return m_str; }

        // === 区域 ===
        void setPosition(const f32 x, const f32 y) { m_x = x; m_y = y; }
        [[nodiscard]] f32 x() const { return m_x; }
        [[nodiscard]] f32 y() const { return m_y; }
        void setSize(const f32 w, const f32 h) { m_w = w; m_h = h; }
        void setWidth(const f32 w) { m_w = w; }
        [[nodiscard]] f32 width() const { return m_w; }
        [[nodiscard]] f32 height() const { return m_h; }

        // === 外观 ===
        void setLineHeight(const f32 h) { m_lineHeight = h; }
        [[nodiscard]] f32 lineHeight() const { return m_lineHeight; }
        void setColor(const ColorRGBA &color) { m_color = color.getSDLFColor(); }

        // === 滚动（InputBox 用） ===
        void setScrollX(const f32 sx) { m_scrollX = sx; }
        [[nodiscard]] f32 scrollX() const { return m_scrollX; }

        // === 换行 ===
        void setWordWrap(const bool on) { m_wordWrap = on; }
        [[nodiscard]] bool wordWrap() const { return m_wordWrap; }

        // === 对齐 ===
        void setTextAlign(const TextAlign a) { m_align = a; }
        [[nodiscard]] TextAlign textAlign() const { return m_align; }
        void setAlignToEnd(const bool on) { m_alignToEnd = on; }
        [[nodiscard]] bool alignToEnd() const { return m_alignToEnd; }

    private:
        f32 m_x, m_y, m_w, m_h, m_lineHeight;
        f32 m_scrollX = 0;
        SDL_FColor m_color;
        utf32str m_str;
        bool m_wordWrap = false;
        bool m_alignToEnd = false;
        TextAlign m_align = TextAlign::Left;
    };
}