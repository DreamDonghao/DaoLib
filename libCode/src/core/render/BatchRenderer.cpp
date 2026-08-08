#include <filesystem>
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/AtlasRegion.hpp>
#include <SDL3_image/SDL_image.h>

namespace dao {
    BatchRenderer::BatchRenderer(const i32 windowID, SDL_Renderer *renderer)
        : m_dynamicBatchGroup(Config::defaultDynamicVerticesCount) {
        if (renderer == nullptr) {
            ErrorLog("渲染器不存在");
            return;
        }
        id = windowID;
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

    void BatchRenderer::render() {
        m_atlas[1] = m_glyphAtlas.getAtlasTexture(id, m_renderer);
        SDL_RenderClear(m_renderer);
        m_dynamicBatchGroup.render(m_renderer, m_atlas);
        for (auto batchGroup = m_staticBatchGroups.begin(); batchGroup != m_staticBatchGroups.end(); ++batchGroup) {
            if (batchGroup->isErased()) {
                batchGroup = m_staticBatchGroups.erase(batchGroup);
                continue;
            }
            batchGroup->render(m_renderer, m_atlas);
        }

        SDL_RenderPresent(m_renderer);
        m_dynamicBatchGroup.clear();
    }

    void BatchRenderer::loadGlyph(const utf32char charCode) const {
        m_glyphAtlas.tryRegisterGlyph(charCode);
    }

    SDL_Vertex *BatchRenderer::allocateVertices(const i32 atlasID, const i32 count) {
        return m_dynamicBatchGroup.allocateVertices(atlasID, count);
    }

    BatchGroup &BatchRenderer::allocateBatchGroup(i32 vertexCount) {
        m_staticBatchGroups.emplace_back(vertexCount);
        return m_staticBatchGroups.back();
    }
}
