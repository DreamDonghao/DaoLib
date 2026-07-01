#include <core/render/primitives/GlyphAtlas.hpp>
#include <core/tool/better_stl.hpp>
std::string dao::GlyphAtlas::s_ttfPath;
dao::f32 dao::GlyphAtlas::s_glyphSize = 32;
dao::i32 dao::GlyphAtlas::s_atlasSize = 5120;
bool dao::GlyphAtlas::s_pixelFont = false;

void dao::GlyphAtlas::configure(const std::string_view ttfPath,const f32 glyphSize, const i32 atlasSize, const bool pixelFont) {
    s_ttfPath = ttfPath;
    s_glyphSize = glyphSize;
    s_atlasSize = atlasSize;
    s_pixelFont = pixelFont;
}


bool dao::GlyphAtlas::isPixelFont() { return s_pixelFont; }

dao::GlyphAtlas::~GlyphAtlas() {
    if (m_atlas) {
        SDL_DestroySurface(m_atlas);
        m_atlas = nullptr;
    }
}

void dao::GlyphAtlas::cleanup() {
    auto &self = getGlyphAtlas();
    if (self.m_font) {
        TTF_CloseFont(self.m_font);
        self.m_font = nullptr;
    }
    TTF_Quit();
}

dao::GlyphAtlas & dao::GlyphAtlas::getGlyphAtlas() {
    static GlyphAtlas glyphAtlas;
    return glyphAtlas;
}

dao::GlyphAtlas::GlyphAtlas()
    : m_glyphSize(s_glyphSize), m_atlasSize(s_atlasSize) {
    static bool ttfReady = false;
    if (!ttfReady) {
        if (!TTF_Init()) {
            ErrorLog("初始化 SDL_TTF 失败");
        }
        ttfReady = true;
    }
    m_font = TTF_OpenFont(s_ttfPath.data(), s_glyphSize);
    if (!m_font) {
        ErrorLog("字体文件加载失败:" + s_ttfPath);
    }
    m_atlas = SDL_CreateSurface(s_atlasSize, s_atlasSize, SDL_PIXELFORMAT_RGBA32);
    SDL_FillSurfaceRect(
        m_atlas, nullptr,
        SDL_MapRGBA(SDL_GetPixelFormatDetails(m_atlas->format), nullptr, 0, 0, 0, 0)
    );
}

void dao::GlyphAtlas::registerGlyph(const utf32char charCode) {
    SDL_Surface *glyphSurface = s_pixelFont
        ? TTF_RenderGlyph_Solid(m_font, charCode, SDL_Color{255, 255, 255, 255})
        : TTF_RenderGlyph_Blended(m_font, charCode, SDL_Color{255, 255, 255, 255});

    // 图集扩容
    if (m_cursor.y + m_cursor.rowHeight + glyphSurface->h > m_atlasSize) {
        const i32 newSize = m_atlasSize * 2;
        SDL_Surface *newAtlas = SDL_CreateSurface(newSize, newSize, SDL_PIXELFORMAT_RGBA32);
        SDL_FillSurfaceRect(newAtlas, nullptr,
                            SDL_MapRGBA(SDL_GetPixelFormatDetails(newAtlas->format), nullptr, 0, 0, 0, 0));
        const SDL_Rect srcRect{0, 0, m_atlasSize, m_atlasSize};
        SDL_BlitSurface(m_atlas, &srcRect, newAtlas, &srcRect);
        SDL_DestroySurface(m_atlas);
        m_atlas = newAtlas;
        m_atlasSize = newSize;
        m_isUpdated = true;
    }

    m_cursor.rowHeight = std::max(m_cursor.rowHeight, glyphSurface->h);

    if (m_cursor.x + glyphSurface->w > m_atlasSize) {
        m_cursor.x = 0;
        m_cursor.y += m_cursor.rowHeight;
        m_cursor.rowHeight = glyphSurface->h;
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

dao::f32 dao::GlyphAtlas::getGlyphAdvance(const utf32char charCode) const {
    if (const auto it = m_glyphs.find(charCode); it != m_glyphs.end()) {
        return it->second.advance;
    }
    return m_glyphSize;
}

dao::f32 dao::GlyphAtlas::getGlyphBearingX(const utf32char charCode) const {
    if (const auto it = m_glyphs.find(charCode); it != m_glyphs.end()) {
        return it->second.bearing.x;
    }
    return 0.0f;
}

bool dao::GlyphAtlas::isLoaded(const utf32char charCode) const {
    return m_glyphs.contains(charCode);
}

