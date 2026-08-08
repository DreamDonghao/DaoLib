#pragma once
#include <SDL3/SDL_render.h>
#include <vector>
#include <core/render/primitives/GlyphAtlas.hpp>
#include "core/tool/Log.hpp"
#include <core/frame/Config.hpp>
#include <list>

namespace dao {
    class BatchGroup {
    public:
        explicit BatchGroup(const i32 vertexCount) {
            m_vertices.resize(vertexCount);
        }

        void render(SDL_Renderer *renderer, const std::vector<SDL_Texture *> &atlas){
            const SDL_Vertex *vertices = m_vertices.data();
            for (auto [atlasId, size]: m_batches) {
                SDL_RenderGeometry(
                    renderer, atlas[atlasId],
                    vertices, size,
                    nullptr, 0
                );
                vertices += size;
            }
        }

        void clear() {
            m_index = 0;
            m_batches.clear();
        }

        SDL_Vertex *allocateVertices(const i32 atlasID, const i32 count) {
            if (m_endAtlasId != atlasID || m_batches.empty()) {
                m_batches.push_back({.atlasId = atlasID, .size = 0});
                m_endAtlasId = atlasID;
            }
            SDL_Vertex *ret = m_vertices.data() + m_index;
            if (m_index + count > m_vertices.size()) {
                ErrorLog("批处理顶点池容量不足");
                std::exit(1);
            }
            m_index += count;
            m_batches.back().size += count;
            return ret;
        }

        [[nodiscard]] bool isErased() const {
            return m_isErased;
        }

        void erase() {
            m_isErased = true;
        }

        struct Batch {
            i32 atlasId;
            i32 size;
        };
    private:
        std::vector<SDL_Vertex> m_vertices; ///< 顶点数据
        std::vector<Batch> m_batches;       ///< 渲染批数据
        i32 m_index{0};                     ///< 将要分配顶点数据数组的位置
        i32 m_endAtlasId{0};                ///< 最后一个纹理的 ID
        bool m_isErased{false};
    };

    /// @brief 批处理渲染器
    /// @details 用于构建纹理与几何顶点的批处理数组
    class BatchRenderer {
    public:
        /// @brief 构造函数（带字体）
        /// @details 创建批处理渲染器，初始化矩形索引缓冲并加载字形图集
        /// @param windowID 对应的窗口 ID
        /// @param renderer SDL渲染器指针，由窗口创建
        explicit BatchRenderer(i32 windowID, SDL_Renderer *renderer);

        /// @brief 禁止拷贝构造函数
        BatchRenderer(const BatchRenderer &) = delete;

        /// @brief 禁止拷贝赋值运算符
        BatchRenderer &operator=(const BatchRenderer &) = delete;

        /// @brief 禁止移动构造
        BatchRenderer(BatchRenderer &&) = delete;

        /// @brief 禁止移动赋值
        BatchRenderer &operator=(BatchRenderer &&) = delete;

        /// @brief 析构函数
        /// @details 清理所有纹理资源和渲染器
        ~BatchRenderer();

        /// @brief 注册纹理到渲染器
        /// @details 根据纹理ID加载对应的纹理图集到GPU，如果已加载则跳过
        /// @param textureId 纹理ID
        void loadAtlas(i32 textureId);

        /// @brief 执行渲染
        /// @details 清空屏幕，按纹理图集分组渲染所有批处理数据，最后呈现到屏幕
        void render();

        void loadGlyph(utf32char charCode) const;

        /// @brief 分配指定数量的顶点空间
        /// @param atlasID 用于渲染的图集 ID
        /// @param count 需要分配的顶点数量
        /// @return 指向分配空间的指针
        /// @note 调用方需确保 count 不会超出剩余容量
        SDL_Vertex *allocateVertices(i32 atlasID, i32 count);

        BatchGroup &allocateBatchGroup(i32 vertexCount);

    private:
        i32 id = -1;
        GlyphAtlas &m_glyphAtlas{GlyphAtlas::getGlyphAtlas()}; ///< 字形图集
        SDL_Renderer *m_renderer{nullptr};                     ///< SDL渲染器指针
        std::vector<SDL_Texture *> m_atlas{2, nullptr};        ///< 图集
        BatchGroup m_dynamicBatchGroup;                        ///< 动态渲染批组
        std::list<BatchGroup> m_staticBatchGroups;             ///< 静态渲染批组
    };
}
