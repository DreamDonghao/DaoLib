#pragma once
#include <core/tool/MappedRegion.hpp>

namespace dao {
    using TextureID = i32;

    /// @brief 图集纹理
    /// @details 表示纹理图集中的一个纹理区域，用于在屏幕上渲染指定的纹理部分
    /// 本类型本身不持有实际纹理数据，只保存纹理ID和显示区域信息
    /// 可通过 getAtlasRegion() 获取纹理在图集中的位置和所属图集信息
    class AtlasTexture {
    public:
        AtlasTexture() = default;

        /// @brief 构造函数，通过指定边界坐标创建纹理
        /// @param textureId 纹理ID
        /// @param left 左边界坐标
        /// @param top 上边界坐标
        /// @param right 右边界坐标
        /// @param bottom 下边界坐标
        AtlasTexture(TextureID textureId, f32 left, f32 top, f32 right, f32 bottom);

        /// @brief 构造函数，通过边界框创建纹理
        /// @param textureId 纹理ID
        /// @param boundingBox 纹理显示边界框
        AtlasTexture(TextureID textureId, BoundingBox boundingBox);

        /// @brief 设置纹理位置（保持大小不变）
        /// @param x 新的左边界坐标
        /// @param y 新的上边界坐标
        void setPosition(f32 x, f32 y);

        /// @brief 设置纹理大小（保持位置不变）
        /// @param width 新的宽度
        /// @param height 新的高度
        void setSize(f32 width, f32 height);

        /// @brief 获取纹理ID
        /// @return 纹理ID
        [[nodiscard]] TextureID getTextureID() const;

        /// @brief 获取显示边界框
        /// @return 纹理显示边界框的常量引用
        [[nodiscard]] const BoundingBox &getBoundingBox() const;

    private:
        TextureID m_textureId = 0; ///< 纹理ID
        BoundingBox m_boundingBox; ///< 纹理在屏幕上的显示区域
    };
}
