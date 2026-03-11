#pragma once
#include <core/tool/better_stl.hpp>
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
            float advance;      ///< 光标前进量
        };

    public:
        GlyphAtlas() = default;

        GlyphAtlas(GlyphAtlas &glyphAtlas) = delete;

        /// @param ttfPath 字体文件
        /// @param glyphSize 字号(磅值)大小,决定渲染效果
        /// @param atlasSize 字形图集大小
        explicit GlyphAtlas(const std::string_view &ttfPath, f32 glyphSize = 24, i32 atlasSize = 1024);

        /// @brief 加载字形
        /// @details 将一个文字的字形编码添加到字形图集中
        /// @param charCode 文字的utf-32编码
        /// @return 是否加载
        bool registerGlyph(utf32char charCode);

        /// @brief 批量加载字形
        /// @details 将一批文字的字形编码添加到字形图集中
        void batchRegisterGlyph(std::string_view chars);

        /// @brief 获取文字在图集中的位置
        BoundingBox getGlyphAtlasRegion(char32_t charCode)const;

        /// @brief 获取字形图集
        [[nodiscard]] SDL_Surface &getAtlasSurface() const {
            return *m_atlasSurface;
        }

        /// @brief 清除添加字形标记
        void clearUpdateFlag() { m_isUpdated = false; }

        /// @brief 获取是否有新的字形添加
        [[nodiscard]] bool isUpdated() const { return m_isUpdated; }


        /// @brief 获取字形的宽高比
        [[nodiscard]] f32 getGlyphAspectRatio(utf32char charCode) const;

    private:
        struct Cursor {
            i32 x, y, rowHeight;
        };

        i32 m_atlasSize{0};                             ///< 字形图集大小
        TTF_Font *m_font{nullptr};                      ///< 字体文件
        SDL_Surface *m_atlasSurface{nullptr};           ///< 字形图集
        hash_map<utf32char, Glyph> m_glyphs{};          ///< 字符在图集中的位置
        hash_map<utf32char, f32> m_glyphAspectRatios{}; ///< 字符宽高比
        Cursor m_cursor{0, 0, 0};
        bool m_isUpdated{false};
    };
}
