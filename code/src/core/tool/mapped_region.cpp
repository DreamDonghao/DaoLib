//
// Created by donghao on 25-12-6.
//
#include <core/tool/mapped_region.hpp>

namespace dao {
    MappedRegion::MappedRegion(const float leftA, const float upA, const float rightA, const float downA,
                    const float leftB, const float upB, const float rightB, const float downB)
           : m_boundingBoxA(leftA, upA, rightA, downA),
             m_boundingBoxB(leftB, upB, rightB, downB) {
    }
}