#pragma once
#include <SDL3/SDL_render.h>
#include <vector>
#include <core/render/primitives/GlyphAtlas.hpp>

namespace dao {
    /// @brief 批处理渲染器
    /// @details 用于构建纹理与几何顶点的批处理数组
    class BatchRenderer {
        struct Batch {
            i32 atlasId;
            i32 size;
        };

    public:
        /// @brief 构造函数（带字体）
        /// @details 创建批处理渲染器，初始化矩形索引缓冲并加载字形图集
        /// @param verticesCount 顶点池默认最大顶点数
        /// @param fontPath 字体文件路径
        /// @param glyphSize 字形大小（像素）
        /// @param atlasSize 字形图集尺寸（正方形边长）
        explicit BatchRenderer(i32 verticesCount, std::string_view fontPath, f32 glyphSize = 32, i32 atlasSize = 1024);

        /// @brief 析构函数
        /// @details 清理所有纹理资源和渲染器
        ~BatchRenderer();

        /// @brief 初始化渲染器
        /// @details 设置渲染器并创建默认纹理（白色纹理和字形图集纹理）
        /// @param renderer SDL渲染器指针，由窗口创建
        void init(SDL_Renderer *renderer);

        /// @brief 禁用复制构造函数
        BatchRenderer(const BatchRenderer &) = delete;

        /// @brief 清理要绘制的纹理图集
        /// @details 一般要每帧调用，否则会堆积上一帧的内容
        void clear();

        /// @brief 注册纹理到渲染器
        /// @details 根据纹理ID加载对应的纹理图集到GPU，如果已加载则跳过
        /// @param textureId 纹理ID
        void loadAtlas(i32 textureId);

        /// @brief 执行渲染
        /// @details 清空屏幕，按纹理图集分组渲染所有批处理数据，最后呈现到屏幕
        void render();

        /// @brief 获取字形图集
        /// @return 字形图集的常量引用
        [[nodiscard]] const GlyphAtlas &getGlyphAtlas() const { return m_glyphAtlas; }

        void loadGlyph(utf32char charCode);

        /// @brief 分配指定数量的顶点空间
        /// @param atlasID 用于渲染的图集 ID
        /// @param count 需要分配的顶点数量
        /// @return 指向分配空间的指针
        /// @note 调用方需确保 count 不会超出剩余容量
        SDL_Vertex *allocateVertices(i32 atlasID, i32 count);
    private:
        SDL_Renderer *m_renderer{nullptr}; ///< SDL渲染器指针
        GlyphAtlas m_glyphAtlas;           ///< 字形图集
        std::vector<SDL_Texture *> m_atlas{2, nullptr};
        std::vector<SDL_Vertex> m_vertices;
        std::vector<Batch> m_batches;
        i32 m_index{0};
        i32 m_endAtlasId{0};
    };
}
