//
// Created by donghao on 25-11-28.
//
#ifndef IMAGE_HPP
#define IMAGE_HPP
#include "core/tool/mapped_region.hpp"

namespace dao {
    using TextureID = i32;

    /// @brief 图集纹理
    /// @details 表示纹理图集中的一个纹理区域
    /// 本类型本身不持有实际纹理数据，只保存纹理名称的枚举值
    /// 可通过 getAtlasRegion() 获取其所在图集及在图集中的区域数据
    class AtlasTexture {
    public:
        AtlasTexture() = default;

        /// @brief 构造函数
        /// @param textureId 纹理枚举名
        /// @param left 左边界坐标
        /// @param up 上区边界标
        /// @param right 右边界坐标
        /// @param down 下边界坐标
        AtlasTexture(i32 textureId, float left, float up, float right, float down);

        AtlasTexture(i32 textureId,BoundingBox boundingBox);

        /// @brief 设置位置
        void setPosition(f32 x, f32 y);

        /// @brief 设置大小
        void setSize(f32 width, f32 height);

        /// @brief 获取枚举名
        /// @returns 纹理枚举名
        [[nodiscard]] const i32 &getName() const { return m_textureId; }

        /// @brief 获取显示边界框
        /// @returns 边界框
        [[nodiscard]] const BoundingBox &getBoundingBox() const { return m_boundingBox; }

    private:
        TextureID m_textureId = 0;    ///< 纹理枚举名
        BoundingBox m_boundingBox; ///< 纹理要显示的位置
    };
}


#endif //IMAGE_HPP
