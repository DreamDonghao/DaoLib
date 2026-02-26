#ifndef ATLAS_REGION_DATA_HPP
#define ATLAS_REGION_DATA_HPP
#include <core/tool/better_stl.hpp>

namespace dao {
    /// @brief 图集区域数据
    struct AtlasRegion {
        const char *const atlasPath; ///< 图集区域对应的纹理图集文件路径
        const i32 atlasId;           ///< 图集区域对应的纹理图集编号
        const f32 normalizedLeft, normalizedTop,
                normalizedRight, normalizedBottom; ///< 归一化的图集区域的左上右下坐标
    };

    extern const AtlasRegion regions[];

    /// @brief 通过纹理名称枚举获取图集区域数据
    inline const AtlasRegion &getAtlasRegion(const i32 textureId) {
        return regions[textureId];
    }
}

#endif //ATLAS_REGION_DATA_HPP
