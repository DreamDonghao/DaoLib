#pragma once
#include <SDL3/SDL_render.h>
#include <core/render/AtlasTexture.hpp>
#include <utility>
#include <vector>
#include <span>
#include <core/render/primitives/GlyphAtlas.hpp>
#include <core/render/Text.hpp>
#include <core/render/Triangle.hpp>
#include <core/render/primitives/AtlasRegion.hpp>
#include <SDL3_image/SDL_image.h>

namespace dao {
    /// @brief 纹理图集绘制批
    /// @details 一次纹理图集绘制用到的数据
    struct AtlasDrawBatch {
        AtlasDrawBatch(const i32 a, std::vector<SDL_Vertex> v,
                       std::unique_ptr<std::vector<i32>, SwitchDeleter<std::vector<i32> > > i)
            : atlasId(a), vertices(std::move(v)), indices(std::move(i)) {
        }

        i32 atlasId;                                                                  ///< 绘制的纹理图集 ID
        std::vector<SDL_Vertex> vertices;                                             ///< 绘制纹理图集的顶点数组
        std::unique_ptr<std::vector<i32>, SwitchDeleter<std::vector<i32> > > indices; ///< 索引数组（可能共享静态缓冲区或独占）
        int indicesCount{0};                                                          ///< 实际使用的索引数量
    };

    /// @brief 批处理渲染器
    /// @details 用于构建纹理与几何顶点的批处理数组
    class BatchRenderer {
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

        /// @brief 添加纹理的数据到顶点数组
        /// @param vertices 目标顶点数组
        /// @param pos 纹理位置和尺寸
        /// @param atlasRegion 纹理在图集中的区域
        static void appendQuadVertices(std::vector<SDL_Vertex> &vertices, BoundingBox pos,
                                       const AtlasRegion &atlasRegion);

    public:
        /// @brief 构造函数（无字体）
        /// @details 创建批处理渲染器，仅初始化矩形索引缓冲
        /// @param quadCount 初始矩形索引缓冲容量
        explicit BatchRenderer(const size_t quadCount = 1024) { expandQuadIndicesTo(quadCount); }

        /// @brief 构造函数（带字体）
        /// @details 创建批处理渲染器，初始化矩形索引缓冲并加载字形图集
        /// @param fontPath 字体文件路径
        /// @param glyphSize 字形大小（像素）
        /// @param atlasSize 字形图集尺寸（正方形边长）
        /// @param quadCount 初始矩形索引缓冲容量
        explicit BatchRenderer(std::string_view fontPath, f32 glyphSize = 24,
                               i32 atlasSize = 1024,
                               size_t quadCount = 1024);

        /// @brief 析构函数
        /// @details 清理所有纹理资源和渲染器
        ~BatchRenderer();

        /// @brief 初始化渲染器
        /// @details 设置渲染器并创建默认纹理（白色纹理和字形图集纹理）
        /// @param renderer SDL渲染器指针，由窗口创建
        void init(SDL_Renderer *renderer);

        /// @brief 禁用复制构造函数
        BatchRenderer(const BatchRenderer &) = delete;

        // @brief 添加原始顶点和索引数据到批处理
        // @details 将用户提供的顶点和索引数据直接添加到渲染批次中。
        //          使用 atlasId = 0（白色纹理），适用于纯色几何图元渲染。
        //          索引值会根据当前批次已有的顶点数自动偏移。
        // @param v 顶点数据数组
        // @param indices 索引数据数组（索引基于顶点数组的相对位置）
        void addToBatch(std::span<const SDL_Vertex> v, std::span<const i32> indices);

        /// @brief 添加三角形几何图元到批处理
        /// @param triangle 三角形图元
        void addToBatch(const Triangle &triangle);

        /// @brief 添加一组三角形几何图元到批处理
        /// @param triangles 三角形图元数组
        void addToBatch(const std::span<const Triangle> &triangles);

        /// @brief 添加纹理到批处理
        /// @param texture 要添加的纹理图集纹理
        void addToBatch(const AtlasTexture &texture);

        /// @brief 添加一组纹理到批处理
        /// @param textures 纹理图集纹理数组
        void addToBatch(const std::span<const AtlasTexture> &textures);

        /// @brief 添加文本到批处理
        /// @param text 要添加的文本对象
        void addToBatch(const Text &text);

        /// @brief 添加一组文本到批处理
        /// @param texts 文本对象数组
        void addToBatch(const std::span<const Text> &texts);

        /// @brief 清理要绘制的纹理图集
        /// @details 一般要每帧调用，否则会堆积上一帧的内容
        /// @param reserveSize 为下次绘制预留的容量，避免频繁重新分配
        void clearDrawBatches(u64 reserveSize = 0);

        /// @brief 获取将要绘制的所有内容的数据
        /// @return 绘制批次数据列表的常量引用
        [[nodiscard]] const std::vector<AtlasDrawBatch> &getDrawBatches() const { return m_drawBatches; }

        /// @brief 注册纹理到渲染器
        /// @details 根据纹理ID加载对应的纹理图集到GPU，如果已加载则跳过
        /// @param textureId 纹理ID
        void registerTexture(const i32 &textureId);

        /// @brief 执行渲染
        /// @details 清空屏幕，按纹理图集分组渲染所有批处理数据，最后呈现到屏幕
        void render();

        /// @brief 获取字形图集
        /// @return 字形图集的常量引用
        [[nodiscard]] const GlyphAtlas &getGlyphAtlas() const { return m_glyphAtlas; }

    private:
        SDL_Renderer *m_renderer{nullptr};         ///< SDL渲染器指针
        std::vector<AtlasDrawBatch> m_drawBatches; ///< 一组绘制的数据
        /// @brief 纹理图集ID到SDL纹理的映射
        /// @details 预留 ID\n
        /// 0: 白色纹理（用于几何图形绘制）
        /// 1: 字形图集
        hash_map<TextureID, SDL_Texture *> m_atlasTextures;
        GlyphAtlas m_glyphAtlas; ///< 字形图集
    };
}
