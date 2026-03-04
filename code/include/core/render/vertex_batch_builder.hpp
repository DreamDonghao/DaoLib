#pragma once
#include <SDL3/SDL_render.h>
#include <core/render/atlas_texture.hpp>
#include <utility>
#include <vector>
#include <span>
#include <core/render/primitives/glyph_atlas.hpp>
#include <core/render/text.hpp>
#include <core/render/triangle.hpp>

namespace dao {
    /// @brief 纹理图集绘制批
    /// @details 一次纹理图集绘制用到的数据
    struct AtlasDrawBatch {
        AtlasDrawBatch(const i32 a, std::vector<SDL_Vertex> v,
                       std::unique_ptr<std::vector<i32>, SwitchDeleter<std::vector<i32> > > i)
            : atlasId(a), vertices(std::move(v)), indices(std::move(i)) {
        }

        i32 atlasId;                      ///< 绘制的纹理图集 ID
        std::vector<SDL_Vertex> vertices; ///< 绘制纹理图集的顶点数组
        std::unique_ptr<std::vector<i32>, SwitchDeleter<std::vector<i32> > > indices;
        int indicesCount{0};
    };

    /// @brief 顶点批处理构建器
    /// @details 用于构建纹理与几何顶点的批处理数组
    class VertexBatchBuilder {
    public:
        /// @brief 扩容共用矩形索引缓冲
        /// @details 若当前容量不足以容纳指定数量的矩形（每矩形 6 个索引），
        /// 则将索引数组扩展到能容纳 quadCount 个矩形的大小。
        /// 仅在需要时增长容量，不会缩小原有大小。
        /// @param quadCount 需要的矩形数量
        static void expandQuadIndicesTo(size_t quadCount);

        /// @brief 重新设置共用矩形索引缓冲的大小
        /// @details 将索引数组调整为可容纳 quadCount 个矩形的精确大小（每矩形 6 个索引）。
        /// 若当前容量不足则扩容；若容量过大则收缩至精确大小。
        /// @param quadCount 目标矩形数量
        static void resetQuadIndices(size_t quadCount);

    private:
        static std::vector<i32> s_quadIndices; ///< 共用矩形顶点数组索引

        /// @brief 填充矩形索引数据
        /// @param startQuad 起始矩形索引
        /// @param endQuad 结束矩形索引（不含）
        static void fillQuadIndices(size_t startQuad, size_t endQuad);

        /// @brief  添加纹理的数据到顶点数组
        static void appendQuadVertices(std::vector<SDL_Vertex> &vertices, BoundingBox pos, i32 textureId);

    public:
        explicit VertexBatchBuilder(const size_t quadCount = 1024) { expandQuadIndicesTo(quadCount); }

        explicit VertexBatchBuilder(std::string_view fontPath, f32 glyphSize = 24, i32 atlasSize = 1024,
                                    size_t quadCount = 1024);

        /// @brief 禁用复制构造函数
        VertexBatchBuilder(const VertexBatchBuilder &) = delete;

        /// @brief 添加三角形几何图元到批处理
        void addToBatch(const Triangle &triangle);

        /// @brief 添加一组三角形几何图元到批处理
        void addToBatch(const std::span<Triangle> &triangles);

        /// @brief 添加纹理到批处理
        void addToBatch(const AtlasTexture &texture);

        /// @brief 添加一组纹理到批处理
        void addToBatch(const std::span<AtlasTexture> &textures);

        /// @brief 添加文本到批处理
        void addToBatch(const Text &text);

        /// @brief 添加一组文本到批处理
        void addToBatch(const std::span<Text> &texts);

        /// @brief 清理要绘制的纹理图集
        /// @details 一般要每帧调用，否则会堆积上一帧的内容
        void clearDrawBatches(u64 reserveSize = 0);

        /// @brief 获取将要绘制的所有内容的数据
        [[nodiscard]] const std::vector<AtlasDrawBatch> &getDrawBatches() const { return m_drawBatches; }

        [[nodiscard]] GlyphAtlas &getGlyphAtlas() { return m_glyphAtlas; }

    private:
        std::vector<AtlasDrawBatch> m_drawBatches; ///< 一组绘制的数据
        GlyphAtlas m_glyphAtlas;                   ///< 字形图集
    };
}
