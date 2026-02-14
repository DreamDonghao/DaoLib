//
// Created by donghao on 2026/2/12.
//

#include "core/basic_drawing_elements/atlas_region.hpp"

namespace dao {
    const AtlasRegion regions[] = {
        {"", 0, 1, 1, 1, 1},
        {"", 0},
#include <texture_dates.inc>
    };
}
