#pragma once
#include <core/render/AtlasTexture.hpp>
#include <interface/IDrawable.hpp>

namespace dao {
    /// @brief 图像组件
    /// @details 用于在窗口中渲染一个静态图像纹理
    class Image : public ifc::IDrawable {
    public:
        /// @brief 构造函数
        /// @param x 图像左上角 x 坐标
        /// @param y 图像左上角 y 坐标
        /// @param width 图像宽度
        /// @param height 图像高度
        /// @param textureID 纹理图集 ID
        Image(f32 x, f32 y, f32 width, f32 height, TextureID textureID);

        /// @brief 设置图像位置
        /// @param x 新的 x 坐标
        /// @param y 新的 y 坐标
        void setPosition(f32 x, f32 y);

        /// @brief 设置图像大小
        /// @param x 新的宽度
        /// @param y 新的高度
        void setSize(f32 x, f32 y);

        /// @brief 将图像写入批处理渲染器
        /// @param batchRenderer 批处理渲染器引用
        void writeToBatch(BatchRenderer &batchRenderer) const override;

    private:
        AtlasTexture m_texture; ///< 图集纹理
    };
}