#pragma once

namespace terra::math
{
    template<typename T>
    constexpr T abs(T x)
    {
        return x >= 0 ? x : -x;
    }
} // namespace utils
