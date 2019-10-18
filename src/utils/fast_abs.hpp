#pragma once

namespace Utils
{
    template <typename T>
    constexpr T FastAbs(T x)
    {
        return x >= 0 ? x : -x;
    }
}
