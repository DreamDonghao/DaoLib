#pragma once
#include <core/tool/TickTimer.hpp>
#include <core/tool/RingVector.hpp>
#include <core/render/AtlasTexture.hpp>
#include <interface/IDrawable.hpp>
#include <core/tool/better_stl.hpp>

namespace dao {
    /// @brief 动画精灵
    /// @details 用于渲染帧动画，通过在多个纹理之间切换实现动画效果
    class AnimatedSprite : Drawable {
    public:
        /// @brief 构造函数（使用初始化列表）
        /// @param fps 动画帧率
        /// @param x 精灵左上角 x 坐标
        /// @param y 精灵左上角 y 坐标
        /// @param width 精灵宽度
        /// @param height 精灵高度
        /// @param textureIDs 纹理 ID 列表
        AnimatedSprite(const u32 fps, const f32 x, const f32 y, const f32 width, const f32 height,
                       const std::initializer_list<TextureID> &textureIDs)
            : m_tickTimer(std::chrono::milliseconds(1000 / fps)) {
            for (const auto textureID: textureIDs) {
                AtlasTexture texture{textureID, x, y, x + width, y + height};
                m_textures.push_back(texture);
            }
        }

        /// @brief 构造函数（使用数组模板）
        /// @tparam N 数组长度
        /// @param fps 动画帧率
        /// @param x 精灵左上角 x 坐标
        /// @param y 精灵左上角 y 坐标
        /// @param width 精灵宽度
        /// @param height 精灵高度
        /// @param textureIDs 纹理 ID 数组
        template<size_t N>
        AnimatedSprite(const u32 fps, const f32 x, const f32 y, const f32 width, const f32 height,
                       const std::array<TextureID, N> &textureIDs)
            : m_tickTimer(std::chrono::milliseconds(1000 / fps)) {
            for (const auto textureID: textureIDs) {
                AtlasTexture texture{textureID, x, y, x + width, y + height};
                m_textures.push_back(texture);
            }
        }

        /// @brief 构造函数（使用范围）
        /// @param fps 动画帧率
        /// @param x 精灵左上角 x 坐标
        /// @param y 精灵左上角 y 坐标
        /// @param width 精灵宽度
        /// @param height 精灵高度
        /// @param begin 起始纹理 ID
        /// @param end 结束纹理 ID
        AnimatedSprite(const u32 fps, const f32 x, const f32 y, const f32 width, const f32 height,
                       const TextureID begin, const TextureID end)
            : m_tickTimer(std::chrono::milliseconds(1000 / fps)) {
            for (TextureID textureID = begin; textureID < end; ++textureID) {
                AtlasTexture texture{textureID, x, y, x + width, y + height};
                m_textures.push_back(texture);
            }
        }

        /// @brief 开始使用动画，重置计时器
        void beginUse() const {
            m_tickTimer.reset();
        }

        /// @brief 将精灵写入批处理渲染器
        /// @param builder 批处理渲染器引用
        void writeToBatch(BatchRenderer &builder) const override {
            builder.addToBatch(m_textures.val());
            if (m_tickTimer) {
                m_textures.next();
            }
        }

    private:
        mutable RingVector<AtlasTexture> m_textures; ///< 纹理循环队列
        mutable TickTimer m_tickTimer;               ///< 帧动画计时器
    };
}
