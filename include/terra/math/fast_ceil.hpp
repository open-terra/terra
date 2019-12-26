#pragma once

namespace terra
{
    template<typename T>
    constexpr T fast_ceil(double x)
    {
        T xi = static_cast<T>(x);
        return x > xi ? xi + 1 : xi;
    }
} // namespace utils
