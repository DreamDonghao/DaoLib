#pragma once
#include <core/tool/type.hpp>
#include <core/render/BatchRenderer.hpp>

namespace dao {
    class Text {
    public:
        explicit Text(utf32str str) : m_str(std::move(str)) {
        }

        void writeToBatch(BatchRenderer &bathRender) const {
            const auto &glyph = bathRender.getGlyphAtlas();
            const auto vertices = bathRender.allocateVertices(1, 6 * std::ranges::ssize(m_str));

            f32 x = m_x;
            f32 y = m_y;

            for (const auto &ch: m_str) {
                if (ch == U'\n') {
                    y += m_glyphHeight;
                    x = m_x;
                    continue;
                }
                auto glyphRegion = glyph.getGlyphAtlasRegion(ch);
                const f32 width = m_glyphHeight / glyphRegion.getHeight() * glyphRegion.getWidth();
                vertices[0] = {
                    {x, y},
                    {1.0f, 1.0f, 1.0f, 1.0f},
                    {glyphRegion.getLeft(), glyphRegion.getTop()}
                };
                vertices[1] = {
                    {x + width, y},
                    {1.0f, 1.0f, 1.0f, 1.0f},
                    {glyphRegion.getRight(), glyphRegion.getTop()}
                };
                vertices[2] = {
                    {x + width, y + m_glyphHeight},
                    {1.0f, 1.0f, 1.0f, 1.0f},
                    {glyphRegion.getRight(), glyphRegion.getBottom()}
                };
                vertices[4] = {
                    {x, y + m_glyphHeight},
                    {1.0f, 1.0f, 1.0f, 1.0f},
                    {glyphRegion.getLeft(), glyphRegion.getBottom()}
                };
                vertices[3] = vertices[2];
                vertices[5] = vertices[0];

                x += width;
            }
        }

    private:
        f32 m_x{0.0f};
        f32 m_y{0.0f};
        f32 m_glyphHeight{0.0f};
        utf32str m_str;
    };
}
