#include <filesystem>
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/AtlasRegion.hpp>
#include <SDL3_image/SDL_image.h>

namespace dao {
    BatchRenderer::BatchRenderer(const i32 verticesCount)
        :m_vertices(verticesCount) {
    }

    BatchRenderer::~BatchRenderer() {
        // 销毁所有纹理
        for (const auto &atlas: m_atlas) {
            if (atlas) {
                SDL_DestroyTexture(atlas);
            }
        }
        if (m_renderer) {
            SDL_DestroyRenderer(m_renderer);
        }
    }

    void BatchRenderer::init(SDL_Renderer *renderer) {
        if (renderer == nullptr) {
            ErrorLog("渲染器不存在");
            return;
        }
        // 绑定窗口的渲染器
        m_renderer = renderer;
        SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderVSync(m_renderer, 1);

        // 创建一个纯白纹理用来渲染几何图形
        SDL_Texture *tex = SDL_CreateTexture(
            m_renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, 1, 1
        );
        constexpr Uint32 whitePixel = 0xFFFFFFFF;
        SDL_UpdateTexture(tex, nullptr, &whitePixel, 4);
        m_atlas[0] = tex;
        SDL_SetTextureBlendMode(m_atlas[0], SDL_BLENDMODE_BLEND);

        // 添加字形图
        SDL_Surface &glyphSurface = GlyphAtlas::getGlyphAtlas().getAtlasSurface();
        m_atlas[1] = SDL_CreateTextureFromSurface(m_renderer, &glyphSurface);
        SDL_SetTextureBlendMode(m_atlas[1], SDL_BLENDMODE_BLEND);
        SDL_SetTextureScaleMode(m_atlas[1], GlyphAtlas::isPixelFont() ? SDL_SCALEMODE_NEAREST : SDL_SCALEMODE_LINEAR);
        m_glyphTexW = glyphSurface.w;
        m_glyphTexH = glyphSurface.h;
    }

    void BatchRenderer::clear() {
        m_index = 0;
        m_batches.clear();
    }

    void BatchRenderer::loadAtlas(const i32 textureId) {
        const AtlasRegion atlasRegion = getAtlasRegion(textureId);
        const i32 atlasId = atlasRegion.atlasId;
        if (atlasId >= m_atlas.size()) {
            m_atlas.resize(atlasId + 1, nullptr);
        }
        if (m_atlas[atlasId] == nullptr) {
            const char *atlasPath = atlasRegion.atlasPath;
            m_atlas[atlasId] = IMG_LoadTexture(m_renderer, atlasPath);
            SDL_SetTextureBlendMode(m_atlas[atlasId], SDL_BLENDMODE_BLEND);
            if (m_atlas[atlasId] == nullptr) {
                ErrorLog("纹理图集加载失败:" + std::string(atlasPath));
            }
            SDL_SetTextureScaleMode(m_atlas[atlasId], SDL_SCALEMODE_NEAREST);
        }
    }

    void BatchRenderer::syncGlyphAtlas() {
        if (!m_glyphAtlas.isUpdated()) return;

        const SDL_ScaleMode scaleMode = GlyphAtlas::isPixelFont() ? SDL_SCALEMODE_NEAREST : SDL_SCALEMODE_LINEAR;

        SDL_Surface &surface = m_glyphAtlas.getAtlasSurface();
        if (surface.w != m_glyphTexW || surface.h != m_glyphTexH) {
            if (m_atlas[1]) SDL_DestroyTexture(m_atlas[1]);
            m_atlas[1] = SDL_CreateTextureFromSurface(m_renderer, &surface);
            SDL_SetTextureBlendMode(m_atlas[1], SDL_BLENDMODE_BLEND);
            SDL_SetTextureScaleMode(m_atlas[1], scaleMode);
            m_glyphTexW = surface.w;
            m_glyphTexH = surface.h;
        } else {
            SDL_UpdateTexture(m_atlas[1], nullptr, surface.pixels, surface.pitch);
        }
        m_glyphAtlas.clearUpdateFlag();
    }

    void BatchRenderer::render() {
        syncGlyphAtlas();
        SDL_RenderClear(m_renderer);
        const SDL_Vertex *vertices = m_vertices.data();
        for (auto [atlasId, size]: m_batches) {
            SDL_RenderGeometry(
                m_renderer, m_atlas[atlasId],
                vertices, size,
                nullptr, 0
            );
            vertices += size;
        }
        SDL_RenderPresent(m_renderer);
    }

    void BatchRenderer::loadGlyph(const utf32char charCode) {
        m_glyphAtlas.tryRegisterGlyph(charCode);
    }

    SDL_Vertex *BatchRenderer::allocateVertices(const i32 atlasID, const i32 count) {
        if (m_endAtlasId != atlasID || m_batches.empty()) {
            m_batches.push_back({atlasID, 0});
            m_endAtlasId = atlasID;
        }
        SDL_Vertex *const ret = m_vertices.data() + m_index;
        if (m_index + count > m_vertices.size()) {
            m_vertices.resize(m_vertices.size() * 2, SDL_Vertex{});
        }
        m_index += count;
        m_batches.back().size += count;
        return ret;
    }
}
