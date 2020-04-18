#pragma once

#include "../base_types.hpp"
#include "pi.hpp"

namespace terra::math
{
    static const tfloat deg_to_rad = PI / static_cast<tfloat>(180.0); 
    static const tfloat rad_to_deg = static_cast<tfloat>(180.0) / PI;

    constexpr tfloat to_radian(tfloat x)
    {
        return x * deg_to_rad;
    }

    constexpr tfloat to_degrees(tfloat x)
    {
        return x * rad_to_deg;
    }
}
