//
// Created by donghao on 25-12-17.
//
#ifndef GLYPH_ATLAS_HPP
#define GLYPH_ATLAS_HPP
#include <core/tool/better_stl.hpp>
#include <SDL3/SDL_surface.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <core/tool/bounding_box.hpp>

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

        /// @param ttfPath 字体文件
        /// @param glyphSize 字号大小
        /// @param atlasSize 字形图集大小
        GlyphAtlas(const std::string_view &ttfPath, const float32 glyphSize, const int32 atlasSize)
            : m_atlasSize(atlasSize), m_font(TTF_OpenFont(ttfPath.data(), glyphSize)),
              m_atlasSurface(SDL_CreateSurface(atlasSize, atlasSize, SDL_PIXELFORMAT_RGBA32)) {
            SDL_FillSurfaceRect(
                m_atlasSurface, nullptr,
                SDL_MapRGBA(
                    SDL_GetPixelFormatDetails(m_atlasSurface->format),
                    nullptr, 0, 0, 0, 0
                )
            );
        }

        /// @brief 加载字形
        /// @details 将一个文字的字形编码添加到字形图集中
        /// @param charCode 文字的utf-32编码
        void registerGlyph(const char32_t charCode) {
            if (m_glyphs.contains(charCode)) { return; }
            m_isUpdated = true;
            SDL_Surface *glyphSurface = TTF_RenderGlyph_Blended(m_font, charCode, SDL_Color{255, 255, 255, 255});
            m_cursor.rowHeight = std::max(m_cursor.rowHeight, glyphSurface->h);
            if (m_cursor.x + glyphSurface->w > m_atlasSize) {
                m_cursor.x = 0;
                m_cursor.y += m_cursor.rowHeight;
                m_cursor.rowHeight = 0;
            }
            const SDL_Rect dst{m_cursor.x, m_cursor.y, glyphSurface->w, glyphSurface->h};
            SDL_BlitSurface(glyphSurface, nullptr, m_atlasSurface, &dst);
            int minx, maxx, miny, maxy, advance;
            TTF_GetGlyphMetrics(m_font, charCode, &minx, &maxx, &miny, &maxy, &advance);

            Glyph glyph{};
            glyph.size = {static_cast<float>(glyphSurface->w), static_cast<float>(glyphSurface->h)};
            glyph.bearing = {static_cast<float>(minx), static_cast<float>(maxy)};
            glyph.advance = static_cast<float>(advance);

            glyph.pos = {
                ratio(dst.x, m_atlasSize), ratio(dst.y, m_atlasSize),
                ratio(dst.w, m_atlasSize), ratio(dst.h, m_atlasSize)
            };

            m_glyphs.emplace(charCode, glyph);
            m_cursor.x += glyphSurface->w;

            SDL_DestroySurface(glyphSurface);
        }

        /// @brief 批量加载字形
        /// @details 将一批文字的字形编码添加到字形图集中
        void batchRegisterGlyph(std::string_view chars) {
            for (auto &ch: chars) {
                registerGlyph(ch);
            }
        }

        /// @brief 获取文字在图集中的位置
        BoundingBox getGlyphAtlasRegion(const char32_t charCode) {
            return {
                m_glyphs[charCode].pos.x, m_glyphs[charCode].pos.y,
                m_glyphs[charCode].pos.x + m_glyphs[charCode].pos.w,
                m_glyphs[charCode].pos.y + m_glyphs[charCode].pos.h,
            };
        }

        /// @brief 获取字形图集
        [[nodiscard]] SDL_Surface &getAtlasSurface() const {
            return *m_atlasSurface;
        }

        /// @brief 清除添加字形标记
        void clearUpdateFlag() { m_isUpdated = false; }

        /// @brief 获取是否有新的字形添加
        [[nodiscard]] bool isUpdated() const { return m_isUpdated; }

    private:
        struct Cursor {
            int32 x, y, rowHeight;
        };

        hash_map<char32_t, Glyph> m_glyphs{}; ///< 字符在图集中的位置
        int32 m_atlasSize{0};
        TTF_Font *m_font{nullptr};
        SDL_Surface *m_atlasSurface{nullptr};
        Cursor m_cursor{0, 0, 0};
        bool m_isUpdated{false};
    };
}
#endif //GLYPH_ATLAS_HPP
