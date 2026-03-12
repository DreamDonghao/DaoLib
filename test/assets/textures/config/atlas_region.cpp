//
// Created by donghao on 2026/2/12.
//

#include <core/render/primitives/AtlasRegion.hpp>

namespace dao {
    const AtlasRegion regions[] = {
        {"", 0, 1, 1, 1, 1},
        {"", 0},
#include <texture_dates.inc>
    };
}
