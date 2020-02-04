#pragma once

#include "../base_types.hpp"
#include "ceil.hpp"
#include "floor.hpp"

namespace terra::math
{
    template<typename T>
    constexpr T round(tfloat x)
    {
        return x > 0.0 ? floor<T>(x + 0.5) : ceil<T>(x - 0.5);
    }
} // namespace terra::math
