#pragma once

#include <vector>

#include "vec2.hpp"

namespace terra
{
    struct polygon
    {
        size_t centre_idx;
        std::vector<terra::vec2> verticis;
    };
}