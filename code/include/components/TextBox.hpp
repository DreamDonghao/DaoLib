#pragma once
#include <core/tool/type.hpp>
#include <core/tool/BoundingBox.hpp>
#include <core/render/IDrawable.hpp>
#include <core/render/primitives/ColorRGB.hpp>

namespace dao {
    /// @brief 文本框组件
    /// @details 带区域控制的文本容器，支持自动换行和裁剪
    class TextBox : public ifc::IDrawable {
    public:
        /// @brief 构造函数
        /// @param x 文本框左上角 x 坐标
        /// @param y 文本框左上角 y 坐标
        /// @param width 文本框宽度
        /// @param height 文本框高度
        /// @param lineHeight 行高
        /// @param color 文本颜色
        explicit TextBox(const f32 x, const f32 y, const f32 width, const f32 height,
                         const f32 lineHeight, const ColorRGBA &color = White)
            : m_bounds(x, y, x + width, y + height), m_lineHeight(lineHeight), m_color(color.getSDLFColor()) {
        }

        /// @brief 写入批处理渲染器
        /// @param batchRenderer 批处理渲染器
        void writeToBatch(BatchRenderer &batchRenderer) const override {
            const auto &atlas = batchRenderer.getGlyphAtlas();
            auto vertices = batchRenderer.allocateVertices(
                1,
                6 * static_cast<i32>(std::ranges::ssize(m_str))
            );

            // 计算文本总高度
            const f32 totalHeight = calculateTextHeight(atlas);

            // 确定起始 Y 坐标
            f32 startY = m_bounds.getTop();
            if (m_alignToEnd && totalHeight > m_bounds.getHeight()) {
                startY = m_bounds.getBottom() - totalHeight;
            }

            f32 x = m_bounds.getLeft();
            f32 y = startY;

            for (const auto ch: m_str) {
                // 处理换行符
                if (ch == U'\n') {
                    y += m_lineHeight;
                    x = m_bounds.getLeft();
                    // 检查是否超出底部边界
                    if (y + m_lineHeight > m_bounds.getBottom()) break;
                    continue;
                }

                batchRenderer.loadGlyph(ch);
                const auto region = atlas.getGlyphAtlasRegion(ch);
                const f32 width = m_lineHeight / region.getHeight() * region.getWidth();

                // 检查是否超出右边界（自动换行）
                if (m_wordWrap && x + width > m_bounds.getRight()) {
                    y += m_lineHeight;
                    x = m_bounds.getLeft();
                    // 检查是否超出底部边界
                    if (y + m_lineHeight > m_bounds.getBottom()) break;
                }

                // 跳过超出左边界的字符
                if (x + width < m_bounds.getLeft()) {
                    x += width;
                    continue;
                }

                // 计算可见区域的纹理坐标
                const f32 uLeft = region.getLeft();
                const f32 uRight = region.getRight();
                const f32 vTop = region.getTop();
                const f32 vBottom = region.getBottom();

                // 裁剪超出边界的部分
                f32 drawX = x;
                f32 drawWidth = width;
                f32 texLeft = uLeft;
                f32 texRight = uRight;

                // 左边界裁剪
                if (x < m_bounds.getLeft()) {
                    const f32 clipRatio = (m_bounds.getLeft() - x) / width;
                    drawX = m_bounds.getLeft();
                    drawWidth = width * (1.0f - clipRatio);
                    texLeft = uLeft + (uRight - uLeft) * clipRatio;
                }

                // 右边界裁剪
                if (x + width > m_bounds.getRight()) {
                    const f32 clipRatio = (m_bounds.getRight() - x) / width;
                    drawWidth = width * clipRatio;
                    texRight = uLeft + (uRight - uLeft) * clipRatio;
                }

                vertices[0] = {{drawX, y}, m_color, {texLeft, vTop}};
                vertices[1] = {{drawX + drawWidth, y}, m_color, {texRight, vTop}};
                vertices[2] = {{drawX + drawWidth, y + m_lineHeight}, m_color, {texRight, vBottom}};
                vertices[3] = vertices[2];
                vertices[4] = {{drawX, y + m_lineHeight}, m_color, {texLeft, vBottom}};
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
        void setPosition(const f32 x, const f32 y) {
            const f32 width = m_bounds.getWidth();
            const f32 height = m_bounds.getHeight();
            m_bounds = BoundingBox(x, y, x + width, y + height);
        }

        /// @brief 设置尺寸
        void setSize(const f32 width, const f32 height) {
            m_bounds = BoundingBox(m_bounds.getLeft(), m_bounds.getTop(),
                                   m_bounds.getLeft() + width, m_bounds.getTop() + height);
        }

        /// @brief 获取边界框
        [[nodiscard]] const BoundingBox &getBounds() const { return m_bounds; }

        /// @brief 设置行高
        void setLineHeight(const f32 lineHeight) { m_lineHeight = lineHeight; }

        /// @brief 获取行高
        [[nodiscard]] f32 lineHeight() const { return m_lineHeight; }

        /// @brief 设置颜色
        void setColor(const ColorRGBA &color) { m_color = color.getSDLFColor(); }

        /// @brief 设置自动换行
        void setWordWrap(const bool enabled) { m_wordWrap = enabled; }

        /// @brief 获取自动换行状态
        [[nodiscard]] bool wordWrap() const { return m_wordWrap; }

        /// @brief 设置文本对齐模式
        /// @param alignToEnd true 时文本从底部向上渲染（末尾优先），false 时从顶部向下渲染（开头优先）
        void setTextAlignToEnd(const bool alignToEnd) { m_alignToEnd = alignToEnd; }

        /// @brief 获取文本对齐模式
        [[nodiscard]] bool textAlignToEnd() const { return m_alignToEnd; }

    private:
        BoundingBox m_bounds;
        f32 m_lineHeight;
        SDL_FColor m_color;
        utf32str m_str;
        bool m_wordWrap = true;
        bool m_alignToEnd = false; ///< true: 末尾优先显示，false: 开头优先显示

        /// @brief 计算文本总高度
        [[nodiscard]] f32 calculateTextHeight(const GlyphAtlas &atlas) const {
            f32 height = m_lineHeight;
            f32 lineWidth = 0.0f;

            for (const auto ch : m_str) {
                if (ch == U'\n') {
                    height += m_lineHeight;
                    lineWidth = 0.0f;
                    continue;
                }

                const f32 charWidth = atlas.getGlyphAspectRatio(ch) * m_lineHeight;
                lineWidth += charWidth;

                if (m_wordWrap && lineWidth > m_bounds.getWidth()) {
                    height += m_lineHeight;
                    lineWidth = charWidth;
                }
            }

            return height;
        }
    };
}