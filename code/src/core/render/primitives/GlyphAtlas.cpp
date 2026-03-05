#include <core/render/primitives/GlyphAtlas.hpp>

dao::GlyphAtlas::GlyphAtlas(const std::string_view &ttfPath, const f32 glyphSize, const i32 atlasSize): m_atlasSize(atlasSize), m_font(TTF_OpenFont(ttfPath.data(), glyphSize)),
    m_atlasSurface(SDL_CreateSurface(atlasSize, atlasSize, SDL_PIXELFORMAT_RGBA32)) {
    if (!m_font) {
        const std::string errorMsg(ttfPath);
        DAO_ERROR_LOG("字体文件加载失败:" + errorMsg);
    }
    SDL_FillSurfaceRect(
        m_atlasSurface, nullptr,
        SDL_MapRGBA(
            SDL_GetPixelFormatDetails(m_atlasSurface->format),
            nullptr, 0, 0, 0, 0
        )
    );
}

bool dao::GlyphAtlas::registerGlyph(const utf32char charCode) {
    if (m_glyphs.contains(charCode)) { return false; }

    m_isUpdated = true;
    SDL_Surface *glyphSurface = TTF_RenderGlyph_Blended(m_font, charCode, SDL_Color{255, 255, 255, 255});
    m_cursor.rowHeight = std::max(m_cursor.rowHeight, glyphSurface->h);
    m_glyphAspectRatios.emplace(charCode, ratio(glyphSurface->w, m_cursor.rowHeight));

    if (m_cursor.x + glyphSurface->w > m_atlasSize) {
        m_cursor.x = 0;
        m_cursor.y += m_cursor.rowHeight;
        m_cursor.rowHeight = 0;
    }
    const SDL_Rect dst{m_cursor.x, m_cursor.y, glyphSurface->w, glyphSurface->h};
    SDL_BlitSurface(glyphSurface, nullptr, m_atlasSurface, &dst);
    int minx, maxx, miny, maxy, advance;
    TTF_GetGlyphMetrics(m_font, charCode, &minx, &maxx, &miny, &maxy, &advance);

    Glyph glyph{};
    glyph.size = {static_cast<float>(glyphSurface->w), static_cast<float>(glyphSurface->h)};
    glyph.bearing = {static_cast<float>(minx), static_cast<float>(maxy)};
    glyph.advance = static_cast<float>(advance);

    glyph.pos = {
        ratio(dst.x, m_atlasSize), ratio(dst.y, m_atlasSize),
        ratio(dst.w, m_atlasSize), ratio(dst.h, m_atlasSize)
    };

    m_glyphs.emplace(charCode, glyph);
    m_cursor.x += glyphSurface->w;

    SDL_DestroySurface(glyphSurface);
    return true;
}

inline void dao::GlyphAtlas::batchRegisterGlyph(const std::string_view chars) {
    for (auto &ch: chars) {
        registerGlyph(ch);
    }
}

dao::BoundingBox dao::GlyphAtlas::getGlyphAtlasRegion(const char32_t charCode) {
    return {
        m_glyphs[charCode].pos.x, m_glyphs[charCode].pos.y,
        m_glyphs[charCode].pos.x + m_glyphs[charCode].pos.w,
        m_glyphs[charCode].pos.y + m_glyphs[charCode].pos.h,
    };
}

dao::f32 dao::GlyphAtlas::getGlyphAspectRatio(const utf32char charCode) const {
    if (const auto it = m_glyphAspectRatios.find(charCode); it != m_glyphAspectRatios.end()) {
        return it->second;
    }
    return 1;
}
