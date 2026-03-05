#pragma once
#include <core/tool/TickTimer.hpp>
#include <core/tool/RingVector.hpp>
#include <core/render/AtlasTexture.hpp>
#include <interface/drawable.hpp>
#include <core/tool/better_stl.hpp>

namespace dao {
    class AnimatedSprite : Drawable {
    public:
        AnimatedSprite(const u32 fps, const f32 x, const f32 y, const f32 width, const f32 height,
                       const std::initializer_list<TextureID> &textureIDs)
            : m_tickTimer(std::chrono::milliseconds(1000 / fps)) {
            for (const auto textureID: textureIDs) {
                AtlasTexture texture{textureID, x, y, x + width, y + height};
                m_textures.push_back(texture);
            }
        }

        template<size_t N>
        AnimatedSprite(const u32 fps, const f32 x, const f32 y, const f32 width, const f32 height,
                       const std::array<TextureID, N> &textureIDs)
            : m_tickTimer(std::chrono::milliseconds(1000 / fps)) {
            for (const auto textureID: textureIDs) {
                AtlasTexture texture{textureID, x, y, x + width, y + height};
                m_textures.push_back(texture);
            }
        }

        AnimatedSprite(const u32 fps, const f32 x, const f32 y, const f32 width, const f32 height,
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

        void writeToBatch(BatchRenderer &builder) const override {
            builder.addToBatch(m_textures.val());
            if (m_tickTimer) {
                m_textures.next();
            }
        }

    private:
        mutable RingVector<AtlasTexture> m_textures;
        mutable TickTimer m_tickTimer;
    };
}
