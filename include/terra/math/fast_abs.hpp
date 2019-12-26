#pragma once

namespace terra
{
    template<typename T>
    constexpr T fast_abs(T x)
    {
        return x >= 0 ? x : -x;
    }
} // namespace utils
