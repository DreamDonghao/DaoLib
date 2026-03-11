#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/AtlasRegion.hpp>
#include <SDL3_image/SDL_image.h>
namespace dao {
    BatchRenderer::BatchRenderer(
        const std::string_view fontPath, const f32 glyphSize, const i32 atlasSize)
        : m_glyphAtlas(fontPath, glyphSize, atlasSize) {
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
            DAO_ERROR_LOG("渲染器不存在");
            return;
        }
        m_renderer = renderer;
        SDL_SetRenderVSync(m_renderer, 0);

        SDL_Texture *tex = SDL_CreateTexture(
            m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 1, 1
        );
        constexpr Uint32 whitePixel = 0xFFFFFFFF;
        SDL_UpdateTexture(tex, nullptr, &whitePixel, 4);
        m_atlas[0] = tex;

        m_atlas[1] = SDL_CreateTextureFromSurface(
            m_renderer, &m_glyphAtlas.getAtlasSurface()
        );
        m_atlas.resize(m_atlas.size(), nullptr);
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
            if (m_atlas[atlasId] == nullptr) {
                DAO_ERROR_LOG("纹理图集加载失败:" + std::string(atlasPath));
            }
            SDL_SetTextureScaleMode(m_atlas[atlasId], SDL_SCALEMODE_NEAREST);
        }
    }

    void BatchRenderer::render() {
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

    SDL_Vertex * BatchRenderer::allocateVertices(const i32 atlasID, const i32 count) {
        if (m_endAtlasId != atlasID || m_batches.empty()) {
            m_batches.push_back({atlasID, 0});
        }
        SDL_Vertex *const ret = m_vertices.data() + m_index;
        m_index += count;
        m_batches.back().size += count;
        return ret;
    }
}
