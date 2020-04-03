#pragma once

#include "../base_types.hpp"

namespace terra::math
{
    template<typename T>
    constexpr T lerp(T a, T b, T w)
    {
        return (a * (static_cast<T>(1) - w)) + (b * w);
    }
} // namespace terra::math
