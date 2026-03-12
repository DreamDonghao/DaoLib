#include <core/render/primitives/GlyphAtlas.hpp>

dao::GlyphAtlas::~GlyphAtlas() {
    if (m_font) {
        TTF_CloseFont(m_font);
    }
    if (m_atlas) {
        SDL_DestroySurface(m_atlas);
    }
}

dao::GlyphAtlas::GlyphAtlas(const std::string_view &ttfPath, const f32 glyphSize, const i32 atlasSize)
    : m_atlasSize(atlasSize), m_font(TTF_OpenFont(ttfPath.data(), glyphSize)),
      m_atlas(SDL_CreateSurface(atlasSize, atlasSize, SDL_PIXELFORMAT_RGBA32)) {
    if (!m_font) {
        const std::string errorMsg(ttfPath);
        DAO_ERROR_LOG("字体文件加载失败:" + errorMsg);
    }
    SDL_FillSurfaceRect(
        m_atlas, nullptr,
        SDL_MapRGBA(SDL_GetPixelFormatDetails(m_atlas->format), nullptr, 0, 0, 0, 0)
    );
}

void dao::GlyphAtlas::registerGlyph(const utf32char charCode) {
    SDL_Surface *glyphSurface = TTF_RenderGlyph_Blended(m_font, charCode, SDL_Color{255, 255, 255, 255});
    m_cursor.rowHeight = std::max(m_cursor.rowHeight, glyphSurface->h);
    m_glyphAspectRatios.emplace(charCode, ratio(glyphSurface->w, m_cursor.rowHeight));

    if (m_cursor.x + glyphSurface->w > m_atlasSize) {
        m_cursor.x = 0;
        m_cursor.y += m_cursor.rowHeight;
        m_cursor.rowHeight = 0;
    }
    const SDL_Rect dst{m_cursor.x, m_cursor.y, glyphSurface->w, glyphSurface->h};
    SDL_BlitSurface(glyphSurface, nullptr, m_atlas, &dst);
    int minx, maxx, miny, maxy, advance;
    TTF_GetGlyphMetrics(m_font, charCode, &minx, &maxx, &miny, &maxy, &advance);

    Glyph glyph{};
    glyph.size = {static_cast<f32>(glyphSurface->w), static_cast<f32>(glyphSurface->h)};
    glyph.bearing = {static_cast<f32>(minx), static_cast<f32>(maxy)};
    glyph.advance = static_cast<f32>(advance);

    glyph.pos = {
        ratio(dst.x, m_atlasSize), ratio(dst.y, m_atlasSize),
        ratio(dst.w, m_atlasSize), ratio(dst.h, m_atlasSize)
    };

    m_glyphs.emplace(charCode, glyph);
    m_cursor.x += glyphSurface->w;

    SDL_DestroySurface(glyphSurface);
    m_isUpdated = true;
}

bool dao::GlyphAtlas::tryRegisterGlyph(const utf32char charCode) {
    if (m_glyphs.contains(charCode)) {
        return false;
    }
    registerGlyph(charCode);
    return true;
}

void dao::GlyphAtlas::batchRegisterGlyph(const utf32str_view chars) {
    for (auto &ch: chars) {
        registerGlyph(ch);
    }
}

dao::BoundingBox dao::GlyphAtlas::getGlyphAtlasRegion(const utf32char charCode) const {
    if (const auto it = m_glyphs.find(charCode); it != m_glyphs.end()) {
        const auto &glyph = it->second;
        return {
            glyph.pos.x, glyph.pos.y,
            glyph.pos.x + glyph.pos.w,
            glyph.pos.y + glyph.pos.h,
        };
    }
    return BoundingBox{};
}

SDL_Surface &dao::GlyphAtlas::getAtlasSurface() const {
    return *m_atlas;
}

dao::f32 dao::GlyphAtlas::getGlyphAspectRatio(const utf32char charCode) const {
    if (const auto it = m_glyphs.find(charCode); it != m_glyphs.end()) {
        return ratio(it->second.size.x, it->second.size.y);
    }
    return 1.0f;
}

bool dao::GlyphAtlas::isLoaded(const utf32char charCode) const {
    return m_glyphs.contains(charCode);
}
