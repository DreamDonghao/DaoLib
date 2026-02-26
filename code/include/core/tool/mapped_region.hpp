#pragma once
#include <core/tool/bounding_box.hpp>
#include <tuple>

namespace dao {
    /// @brief 映射区域
    /// @details 一个边界框（左上右下边界坐标）到另一个边界框（左上右下边界坐标）的映射
    class MappedRegion {
    public:
        MappedRegion() = default;

        MappedRegion(const BoundingBox &boundingBoxA, const BoundingBox &boundingBoxB)
            : m_boundingBoxA(boundingBoxA), m_boundingBoxB(boundingBoxB) {
        }

        MappedRegion(float leftA, float upA, float rightA, float downA,
                     float leftB, float upB, float rightB, float downB);

        [[nodiscard]] std::tuple<BoundingBox, BoundingBox> getBoundingBoxs() const {
            return {m_boundingBoxA, m_boundingBoxB};
        }

    private:
        BoundingBox m_boundingBoxA{0.f, 0.f, 0.f, 0.f};
        BoundingBox m_boundingBoxB{0.f, 0.f, 0.f, 0.f};
    };
}

