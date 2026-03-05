#include <ranges>
#include <span>
#include <core/render/BatchRenderer.hpp>
#include <core/render/primitives/AtlasRegion.hpp>

namespace dao {
    std::vector<int> BatchRenderer::s_quadIndices = {};

    BatchRenderer::BatchRenderer(

        const std::string_view fontPath, const f32 glyphSize,
        const i32 atlasSize, const size_t quadCount)
        : m_glyphAtlas(fontPath, glyphSize, atlasSize) {
        expandQuadIndicesTo(quadCount);
    }

    BatchRenderer::~BatchRenderer() {
        // 销毁所有纹理
        for (const auto &texture: m_atlasTextures | std::views::values) {
            if (texture) {
                SDL_DestroyTexture(texture);
            }
        }
        m_atlasTextures.clear();

        if (m_renderer) { SDL_DestroyRenderer(m_renderer); }
    }

    void BatchRenderer::fillQuadIndices(const size_t startQuad, const size_t endQuad) {
        for (size_t i = startQuad; i < endQuad; ++i) {
            const int base = static_cast<int>(i) * 4;
            s_quadIndices[i * 6 + 0] = base + 0;
            s_quadIndices[i * 6 + 1] = base + 1;
            s_quadIndices[i * 6 + 2] = base + 2;
            s_quadIndices[i * 6 + 3] = base + 2;
            s_quadIndices[i * 6 + 4] = base + 3;
            s_quadIndices[i * 6 + 5] = base + 0;
        }
    }

    void BatchRenderer::expandQuadIndicesTo(const size_t quadCount) {
        if (quadCount * 6 > s_quadIndices.size()) {
            const size_t oldQuadCount = s_quadIndices.size() / 6;
            s_quadIndices.resize(quadCount * 6);
            fillQuadIndices(oldQuadCount, quadCount);
        }
    }

    void BatchRenderer::resetQuadIndices(const size_t quadCount) {
        if (quadCount * 6 > s_quadIndices.size()) {
            const size_t oldQuadCount = s_quadIndices.size() / 6;
            s_quadIndices.resize(quadCount * 6);
            fillQuadIndices(oldQuadCount, quadCount);
        } else {
            s_quadIndices.resize(quadCount * 6);
            s_quadIndices.shrink_to_fit();
        }
    }

    void BatchRenderer::addToBatch(const AtlasTexture &texture) {
        const AtlasRegion atlasRegion = getAtlasRegion(texture.getTextureID());
        if (const i32 atlasId = atlasRegion.atlasId;
            m_drawBatches.empty() || atlasId != m_drawBatches.back().atlasId
        ) {
            m_drawBatches.emplace_back(atlasId, std::vector<SDL_Vertex>(), makeObserver(&s_quadIndices));
        }
        m_drawBatches.back().indicesCount += 6;
        appendQuadVertices(
            m_drawBatches.back().vertices,
            texture.getBoundingBox(), atlasRegion
        );
    }

    void BatchRenderer::addToBatch(const std::span<const AtlasTexture> &textures) {
        for (const auto &texture: textures) {
            addToBatch(texture);
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
        m_atlasTextures[0] = tex;

        m_atlasTextures[1] = SDL_CreateTextureFromSurface(
            m_renderer, &m_glyphAtlas.getAtlasSurface());
    }

    void BatchRenderer::addToBatch(const Triangle &triangle) {
        // 确保有一个用于几何图元的批次（atlasId = 0）
        if (m_drawBatches.empty() || m_drawBatches.back().atlasId != 0) {
            m_drawBatches.emplace_back(0, std::vector<SDL_Vertex>(), makeManage(new std::vector<i32>()));
        }

        auto &batch = m_drawBatches.back();
        const auto offset = static_cast<i32>(batch.vertices.size());

        // 批量添加顶点
        batch.vertices.push_back(static_cast<SDL_Vertex>(triangle.vertex(0)));
        batch.vertices.push_back(static_cast<SDL_Vertex>(triangle.vertex(1)));
        batch.vertices.push_back(static_cast<SDL_Vertex>(triangle.vertex(2)));

        // 批量添加索引
        auto &indices = *batch.indices;
        indices.reserve(indices.size() + 3);
        indices.push_back(offset);
        indices.push_back(offset + 1);
        indices.push_back(offset + 2);

        // 更新索引计数
        batch.indicesCount += 3;
    }

    void BatchRenderer::addToBatch(const std::span<const Triangle> &triangles) {
        for (const auto &triangle: triangles) {
            addToBatch(triangle);
        }
    }

    void BatchRenderer::addToBatch(const Text &text) {
        if (m_drawBatches.empty() || m_drawBatches.back().atlasId != 1) {
            m_drawBatches.emplace_back(1, std::vector<SDL_Vertex>(), makeObserver(&s_quadIndices));
        }
        m_drawBatches.back().indicesCount += static_cast<int>(text.getContent().size()) * 6;
        auto &vertices = m_drawBatches.back().vertices;

        f32 x = text.getX();
        f32 y = text.getY();
        const f32 size = text.getFontSize();
        for (const auto &ch: text.getContent()) {
            if (ch == U'\n') {
                y += size;
                x = text.getX();
                continue;
            }
            if (m_glyphAtlas.registerGlyph(ch)) {
                SDL_UpdateTexture(
                    m_atlasTextures[1], nullptr,
                    m_glyphAtlas.getAtlasSurface().pixels,
                    m_glyphAtlas.getAtlasSurface().pitch
                );
                SDL_SetTextureScaleMode(m_atlasTextures[1], SDL_SCALEMODE_NEAREST);
                m_glyphAtlas.clearUpdateFlag();
            }
            auto b = m_glyphAtlas.getGlyphAtlasRegion(ch);
            const f32 w = size / b.getHeight() * b.getWidth();
            vertices.push_back({
                {x, y},
                text.getColor(),
                {b.getLeft(), b.getTop()}
            });
            vertices.push_back({
                {x + w, y}, text.getColor(), {b.getRight(), b.getTop()}
            });
            vertices.push_back({
                {x + w, y + size}, text.getColor(), {b.getRight(), b.getBottom()}
            });
            vertices.push_back({
                {x, y + size}, text.getColor(), {b.getLeft(), b.getBottom()}
            });
            x += w;
        }
    }

    void BatchRenderer::addToBatch(const std::span<const Text> &texts) {
        for (const auto &text: texts) {
            addToBatch(text);
        }
    }

    void BatchRenderer::clearDrawBatches(const u64 reserveSize) {
        m_drawBatches.clear();
        m_drawBatches.reserve(reserveSize);
    }

    void BatchRenderer::registerTexture(const i32 &textureId) {
        const AtlasRegion atlasRegion = getAtlasRegion(textureId);
        if (const i32 atlasId = atlasRegion.atlasId; !m_atlasTextures.contains(atlasId)) {
            const char *texturePath = atlasRegion.atlasPath;
            m_atlasTextures[atlasId] = IMG_LoadTexture(m_renderer, texturePath);
            if (!m_atlasTextures[atlasId]) {
                DAO_ERROR_LOG("纹理图集加载失败:" + std::string(texturePath));
            }
            SDL_SetTextureScaleMode(m_atlasTextures[atlasId], SDL_SCALEMODE_NEAREST);
        }
    }

    void BatchRenderer::render() {
        SDL_RenderClear(m_renderer);
        for (const auto &drawBatch: m_drawBatches) {
            SDL_RenderGeometry(
                m_renderer, m_atlasTextures[drawBatch.atlasId],
                drawBatch.vertices.data(), static_cast<int>(drawBatch.vertices.size()),
                drawBatch.indices->data(), drawBatch.indicesCount
            );
        }
        SDL_RenderPresent(m_renderer);
    }

    void BatchRenderer::appendQuadVertices(std::vector<SDL_Vertex> &vertices, const BoundingBox pos,
                                           const AtlasRegion &atlasRegion) {
        const f32 winL = pos.getLeft();
        const f32 winT = pos.getTop();
        const f32 winR = pos.getRight();
        const f32 winB = pos.getBottom();
        const f32 normalized_texL = atlasRegion.normalizedLeft;
        const f32 normalized_texT = atlasRegion.normalizedTop;
        const f32 normalized_texR = atlasRegion.normalizedRight;
        const f32 normalized_texB = atlasRegion.normalizedBottom;
        vertices.push_back({{winL, winT}, {1, 1, 1, 1}, {normalized_texL, normalized_texT}});
        vertices.push_back({{winR, winT}, {1, 1, 1, 1}, {normalized_texR, normalized_texT}});
        vertices.push_back({{winR, winB}, {1, 1, 1, 1}, {normalized_texR, normalized_texB}});
        vertices.push_back({{winL, winB}, {1, 1, 1, 1}, {normalized_texL, normalized_texB}});
    }
}
