//
// Created by donghao on 2026/2/12.
//
#ifndef TEST_DAO_ANIMATED_SPRITE_HPP
#define TEST_DAO_ANIMATED_SPRITE_HPP
#include <core/tool/tick_timer.hpp>
#include <core/tool/ring_vector.hpp>
#include "../core/basic_drawing_elements/atlas_texture.hpp"
#include "../interface/drawable.hpp"
#include <core/tool/better_stl.hpp>

namespace dao {
    class AnimatedSprite : Drawable {
    public:
        AnimatedSprite(const u32 fps, const float32 x, const float32 y, const float32 width, const float32 height,
                       const std::initializer_list<u32> &textureIDs)
            : m_tickTimer(std::chrono::milliseconds(1000 / fps)) {
            for (const auto textureID: textureIDs) {
                AtlasTexture texture{textureID, x, y, x + width, y + height};
                m_textures.push_back(texture);
            }
        }

        template<size_t N>
        AnimatedSprite(const u32 fps, const float32 x, const float32 y, const float32 width, const float32 height,
                       const std::array<TextureID, N> &textureIDs)
            : m_tickTimer(std::chrono::milliseconds(1000 / fps)) {
            for (const auto textureID: textureIDs) {
                AtlasTexture texture{textureID, x, y, x + width, y + height};
                m_textures.push_back(texture);
            }
        }

        AnimatedSprite(const u32 fps, const float32 x, const float32 y, const float32 width, const float32 height,
                       const TextureID begin, const TextureID end)
            : m_tickTimer(std::chrono::milliseconds(1000 / fps)) {
            for (TextureID textureID = begin; textureID < end; ++textureID) {
                AtlasTexture texture{textureID, x, y, x + width, y + height};
                m_textures.push_back(texture);
            }
        }

        void beginUse() const {
            m_tickTimer.reset();
        }

        void writeToBatch(VertexBatchBuilder &builder) const override {
            builder.addToBatch(m_textures.val());
            if (m_tickTimer) {
                m_textures.next();
            }
        }

    private:
        mutable RingVector<AtlasTexture> m_textures;
        mutable TickTimer m_tickTimer;
    };
} // dao

#endif //TEST_DAO_ANIMATED_SPRITE_HPP
