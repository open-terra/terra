#pragma once

#include "ceil.hpp"
#include "floor.hpp"

namespace terra::math
{
    template<typename T>
    constexpr T round(double x)
    {
        return x > 0.0 ? floor<T>(x + 0.5) : ceil<T>(x - 0.5);
    }
} // namespace utils
