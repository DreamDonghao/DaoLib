#pragma once
#include <core/tool/type.hpp>
#include <core/render/BatchRenderer.hpp>

#include "../core/render/primitives/ColorRGB.hpp"

namespace dao {
    class Text {
    public:
        explicit Text(const f32 x, const f32 y, const f32 height,const ColorRGBA &color, utf32str str)
            : m_x(x), m_y(y), m_glyphHeight(height),m_color(color.getSDLFColor()), m_str(std::move(str)) {
        }

        void writeToBatch(BatchRenderer &bathRender) const {
            const auto &glyph = bathRender.getGlyphAtlas();
            auto vertices = bathRender.allocateVertices(1, 6 * std::ranges::ssize(m_str));

            f32 x = m_x;
            f32 y = m_y;

            for (const auto &ch: m_str) {
                if (ch == U'\n') {
                    y += m_glyphHeight;
                    x = m_x;
                    continue;
                }

                bathRender.loadGlyph(ch);
                auto glyphRegion = glyph.getGlyphAtlasRegion(ch);
                const f32 width = m_glyphHeight / glyphRegion.getHeight() * glyphRegion.getWidth();
                vertices[0] = {
                    {x, y},
                   m_color,
                    {glyphRegion.getLeft(), glyphRegion.getTop()}
                };
                vertices[1] = {
                    {x + width, y},
                   m_color,
                    {glyphRegion.getRight(), glyphRegion.getTop()}
                };
                vertices[2] = {
                    {x + width, y + m_glyphHeight},
                    m_color,
                    {glyphRegion.getRight(), glyphRegion.getBottom()}
                };
                vertices[4] = {
                    {x, y + m_glyphHeight},
                    m_color,
                    {glyphRegion.getLeft(), glyphRegion.getBottom()}
                };
                vertices[3] = vertices[2];
                vertices[5] = vertices[0];
                vertices += 6;
                x += width;
            }
        }

    private:
        f32 m_x{0.0f};
        f32 m_y{0.0f};
        f32 m_glyphHeight{0.0f};
        SDL_FColor m_color;
        utf32str m_str;
    };
}
