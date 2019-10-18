#pragma once

namespace Utils
{
    template <typename T>
    constexpr T FastFloor(double x)
    {
        T xi = static_cast<T>(x);
        return x < xi ? xi - 1 : xi;
    }
}
