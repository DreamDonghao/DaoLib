//
// Created by donghao on 25-12-1.
//
#ifndef TEXTURE_BATCH_BUILDER_HPP
#define TEXTURE_BATCH_BUILDER_HPP
#include <SDL3/SDL_render.h>
#include <core/basic_drawing_elements/atlas_texture.hpp>
#include <utility>
#include <vector>
#include <span>
#include <core/basic_drawing_elements/glyph_atlas.hpp>
#include <core/basic_drawing_elements/text.hpp>

namespace dao {
    /// @brief 纹理图集绘制批
    /// @details 一次纹理图集绘制用到的数据
    struct AtlasDrawBatch {
        AtlasDrawBatch(const uint32 a, std::vector<SDL_Vertex> v,
                       std::unique_ptr<std::vector<int32>, SwitchDeleter<std::vector<int32> > > i)
            : atlasId(a), vertices(std::move(v)), indices(std::move(i)) {
        }

        uint32 atlasId;                   ///< 绘制的纹理图集 ID
        std::vector<SDL_Vertex> vertices; ///< 绘制纹理图集的顶点数组
        std::unique_ptr<std::vector<int32>, SwitchDeleter<std::vector<int32> > > indices;
        int indicesCount{0};
    };

    /// @brief 顶点批处理构建器
    /// @details 用于构建纹理与几何顶点的批处理数组
    class VertexBatchBuilder {
    public:
        explicit VertexBatchBuilder(const size_t qudaCount = 1024) { expandQudaIndicesTo(qudaCount); }

        explicit VertexBatchBuilder(std::string_view fontPath, float32 glyphSize, int32 atlasSize,
                                    size_t qudaCount = 1024);

        /// @brief 禁用复制构造函数
        VertexBatchBuilder(const VertexBatchBuilder &) = delete;

        /// @brief 扩容共用矩形索引缓冲
        /// @details 若当前容量不足以容纳指定数量的矩形（每矩形 6 个索引），
        /// 则将索引数组扩展到能容纳 qudaCount 个矩形的大小。
        /// 仅在需要时增长容量，不会缩小原有大小。
        /// @param qudaCount 需要的矩形数量
        static void expandQudaIndicesTo(size_t qudaCount);

        /// @brief 重新设置共用矩形索引缓冲的大小
        /// @details 将索引数组调整为可容纳 qudaCount 个矩形的精确大小（每矩形 6 个索引）。
        /// 若当前容量不足则扩容；若容量过大则收缩至精确大小。
        /// @param qudaCount 目标矩形数量
        static void resetQudaIndices(size_t qudaCount);

        /// @brief 添加纹理到批处理
        void addToBatch(const AtlasTexture &texture);

        /// @brief 添加绘制元素到批处理
        void addToBatch(std::span<const SDL_Vertex> v, std::span<const int32> indices);

        /// @brief 添加文本到批处理
        void addToBatch(const Text &text);

        /// @brief 清理要绘制的纹理图集
        /// @details 一般要每帧调用，否则会堆积上一帧的内容
        void clearDrawBatches() { m_drawBatches.clear(); }

        /// @brief 获取将要绘制的所有内容的数据
        [[nodiscard]] const std::vector<AtlasDrawBatch> &getDrawBatches() const { return m_drawBatches; }

        GlyphAtlas &getGlyphAtlas() { return m_glyphAtlas; }

    private:
        std::vector<AtlasDrawBatch> m_drawBatches; ///< 一组绘制的数据
        static std::vector<int32> s_qudaIndices;   ///< 共用矩形顶点数组索引
        GlyphAtlas m_glyphAtlas;

        /// @brief  添加纹理的数据到顶点数组
        static void appendQuadVertices(std::vector<SDL_Vertex> &vertices, BoundingBox pos, uint32 textureId);
    };
} // name_

#endif //TEXTURE_BATCH_BUILDER_HPP
