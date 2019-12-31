#pragma once

namespace terra::math
{
    template<typename T>
    constexpr T ceil(double x)
    {
        T xi = static_cast<T>(x);
        return x > xi ? xi + 1 : xi;
    }
} // namespace terra::math
