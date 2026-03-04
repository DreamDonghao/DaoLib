#pragma once

#include <core/tool/better_stl.hpp>

namespace dao {
    /// @brief 图集区域数据
    /// @details 描述纹理图集中的一个特定区域，包含纹理路径、图集ID和归一化坐标
    struct AtlasRegion {
        const char *const atlasPath; ///< 图集区域对应的纹理图集文件路径
        const i32 atlasId;           ///< 图集区域对应的纹理图集编号
        const f32 normalizedLeft;    ///< 归一化左边界坐标 (0.0 ~ 1.0)
        const f32 normalizedTop;     ///< 归一化上边界坐标 (0.0 ~ 1.0)
        const f32 normalizedRight;   ///< 归一化右边界坐标 (0.0 ~ 1.0)
        const f32 normalizedBottom;  ///< 归一化下边界坐标 (0.0 ~ 1.0)
    };

    /// @brief 图集区域数据数组
    /// @details 存储所有预定义的纹理图集区域，通过纹理ID索引访问
    extern const AtlasRegion regions[];

    /// @brief 通过纹理ID获取对应的图集区域数据
    /// @param textureId 纹理ID，用作regions数组的索引
    /// @return 对应纹理ID的AtlasRegion常量引用
    /// @warning 调用者需确保textureId在有效范围内，否则行为未定义
    inline const AtlasRegion &getAtlasRegion(const i32 textureId) noexcept {
        // 注意：未进行边界检查以提高性能，调用者需确保textureId有效
        return regions[textureId];
    }
}
