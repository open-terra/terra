#pragma once

#include "fast_ceil.hpp"
#include "fast_floor.hpp"

namespace utils
{
    template <typename T>
    constexpr T fast_round(double x)
    {
        return x > 0.0 ? fast_floor<T>(x + 0.5)  : fast_ceil<T>(x - 0.5);
    }
}
