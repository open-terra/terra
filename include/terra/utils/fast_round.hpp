#pragma once

#include "fast_ceil.hpp"
#include "fast_floor.hpp"

namespace Utils
{
    template <typename T>
    constexpr T FastRound(double x)
    {
        return x > 0.0 ? FastFloor<T>(x + 0.5)  : FastCeil<T>(x - 0.5);
    }
}
