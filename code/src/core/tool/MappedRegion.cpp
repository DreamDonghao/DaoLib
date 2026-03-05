//
// Created by donghao on 25-12-6.
//
#include <core/tool/MappedRegion.hpp>

namespace dao {
    MappedRegion::MappedRegion(const f32 leftA, const f32 topA, const f32 rightA, const f32 bottomA,
                    const f32 leftB, const f32 topB, const f32 rightB, const f32 bottomB)
           : m_boundingBoxA(leftA, topA, rightA, bottomA),
             m_boundingBoxB(leftB, topB, rightB, bottomB) {
    }
}