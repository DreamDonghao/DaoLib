#pragma once
#include <SDL3/SDL_surface.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <core/tool/BoundingBox.hpp>

namespace dao {
    /// @brief 字形图集
    /// @details 存储用到文字的图集
    class GlyphAtlas {
        /// @brief 一个字形的数据
        struct Glyph {
            SDL_FRect pos;      ///< 在 atlas 中的位置（0~1）
            SDL_FPoint size;    ///< 位图大小（像素）
            SDL_FPoint bearing; ///< 相对基线的偏移
            f32 advance;        ///< 光标前进量
        };

        struct Cursor {
            i32 x, y, rowHeight;
        };

    public:
        /// @brief 禁用拷贝构造
        GlyphAtlas(const GlyphAtlas &) = delete;

        /// @brief 禁用移动构造
        GlyphAtlas(GlyphAtlas &&) = delete;

        /// @brief 禁用拷贝赋值
        GlyphAtlas &operator=(const GlyphAtlas &) = delete;

        /// @brief 禁用移动赋值
        GlyphAtlas &operator=(GlyphAtlas &&) = delete;

        ~GlyphAtlas();

        static GlyphAtlas &getGlyphAtlas();

        /// @brief 加载字形
        /// @details 将一个文字的字形编码添加到字形图集中
        /// @param charCode 文字的utf-32编码
        void registerGlyph(utf32char charCode);

        /// @brief
        /// @details
        /// @param charCode
        bool tryRegisterGlyph(utf32char charCode);

        /// @brief 批量加载字形
        /// @details 将一批文字的字形编码添加到字形图集中
        void batchRegisterGlyph(utf32str_view chars);

        /// @brief 获取文字在图集中的位置
        [[nodiscard]] BoundingBox getGlyphAtlasRegion(char32_t charCode) const;

        /// @brief 获取字形图集
        [[nodiscard]] SDL_Surface &getAtlasSurface() const;

        /// @brief 清除添加字形标记
        void clearUpdateFlag() { m_isUpdated = false; }

        /// @brief 获取是否有新的字形添加
        [[nodiscard]] bool isUpdated() const { return m_isUpdated; }

        /// @brief 获取字形的宽高比
        [[nodiscard]] f32 getGlyphAspectRatio(utf32char charCode) const;

        /// @brief 获取字形水平 advance（已归一化到图集坐标系）
        [[nodiscard]] f32 getGlyphAdvance(utf32char charCode) const;

        /// @brief 获取字形 bearing.x（已归一化到图集坐标系）
        [[nodiscard]] f32 getGlyphBearingX(utf32char charCode) const;

        /// @brief 获取字体磅值
        [[nodiscard]] f32 getFontSize() const { return m_glyphSize; }

        /// @brief 是否已经加载该文字
        [[nodiscard]] bool isLoaded(utf32char charCode) const;

        /// @brief 在构造前配置字形图集参数
        /// @param ttfPath 字体文件
        /// @param ttfPath 字体文件地址
        /// @param glyphSize 渲染磅值 — 标准字体 32~64，像素字体用其设计尺寸（如 12/16）
        /// @param atlasSize 图集尺寸 — 标准字体 5120，像素字体 256~512 够用
        /// @param pixelFont true→NEAREST 过滤 + 关闭抗锯齿，适合像素字体
        static void configure(std::string_view ttfPath, f32 glyphSize, i32 atlasSize, bool pixelFont = false);

        static void cleanup();

        [[nodiscard]] static bool isPixelFont();

    private:
        static std::string s_ttfPath;
        static f32 s_glyphSize;
        static i32 s_atlasSize;
        static bool s_pixelFont;

        f32 m_glyphSize{32};                            ///< 字体磅值
        i32 m_atlasSize{0};                             ///< 字形图集大小
        TTF_Font *m_font{nullptr};                      ///< 字体文件
        SDL_Surface *m_atlas{nullptr};                  ///< 字形图集
        hash_map<utf32char, Glyph> m_glyphs{};          ///< 字符在图集中的位置
        Cursor m_cursor{0, 0, 0};
        bool m_isUpdated{false};

        explicit GlyphAtlas();
    };
}
