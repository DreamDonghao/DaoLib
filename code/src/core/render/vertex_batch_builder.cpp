#include <span>
#include <core/render/vertex_batch_builder.hpp>
#include <core/render/primitives/atlas_region.hpp>

namespace dao {
    std::vector<int> VertexBatchBuilder::s_quadIndices = {};

    VertexBatchBuilder::VertexBatchBuilder(
        const std::string_view fontPath, const f32 glyphSize,
        const i32 atlasSize, const size_t quadCount)
        : m_glyphAtlas(fontPath, glyphSize, atlasSize) {
        expandQuadIndicesTo(quadCount);
    }

    void VertexBatchBuilder::fillQuadIndices(const size_t startQuad, const size_t endQuad) {
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

    void VertexBatchBuilder::expandQuadIndicesTo(const size_t quadCount) {
        if (quadCount * 6 > s_quadIndices.size()) {
            const size_t oldQuadCount = s_quadIndices.size() / 6;
            s_quadIndices.resize(quadCount * 6);
            fillQuadIndices(oldQuadCount, quadCount);
        }
    }

    void VertexBatchBuilder::resetQuadIndices(const size_t quadCount) {
        if (quadCount * 6 > s_quadIndices.size()) {
            const size_t oldQuadCount = s_quadIndices.size() / 6;
            s_quadIndices.resize(quadCount * 6);
            fillQuadIndices(oldQuadCount, quadCount);
        } else {
            s_quadIndices.resize(quadCount * 6);
            s_quadIndices.shrink_to_fit();
        }
    }

    void VertexBatchBuilder::addToBatch(const AtlasTexture &texture) {
        const AtlasRegion atlasRegion = getAtlasRegion(texture.getTextureID());
        if (const i32 atlasId = atlasRegion.atlasId;
            m_drawBatches.empty() || atlasId != m_drawBatches.back().atlasId
        ) {
            m_drawBatches.emplace_back(atlasId, std::vector<SDL_Vertex>(), makeObserver(&s_quadIndices));
        }
        m_drawBatches.back().indicesCount += 6;
        appendQuadVertices(
            m_drawBatches.back().vertices,
            texture.getBoundingBox(), texture.getTextureID()
        );
    }

    void VertexBatchBuilder::addToBatch(const std::span<AtlasTexture> &textures) {
        for (const auto &texture: textures) {
            addToBatch(texture);
        }
    }

    void VertexBatchBuilder::addToBatch(const Triangle &triangle) {
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

    void VertexBatchBuilder::addToBatch(const std::span<Triangle> &triangles) {
        for (const auto &triangle: triangles) {
            addToBatch(triangle);
        }
    }

    void VertexBatchBuilder::addToBatch(const Text &text) {
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
            m_glyphAtlas.registerGlyph(ch);
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

    void VertexBatchBuilder::addToBatch(const std::span<Text> &texts) {
        for (const auto &text: texts) {
            addToBatch(text);
        }
    }

    void VertexBatchBuilder::clearDrawBatches(const u64 reserveSize) {
        m_drawBatches.clear();
        m_drawBatches.reserve(reserveSize);
    }

    void VertexBatchBuilder::appendQuadVertices(std::vector<SDL_Vertex> &vertices, const BoundingBox pos,
                                                const i32 textureId) {
        const f32 winL = pos.getLeft();
        const f32 winT = pos.getTop();
        const f32 winR = pos.getRight();
        const f32 winB = pos.getBottom();
        const AtlasRegion atlasRegion = getAtlasRegion(textureId);
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
