#pragma once

#include <cstdint>

namespace Utils
{
    template <typename T>
    inline T FastFloor(double x)
    {
        T xi = static_cast<T>(x);
        return x < xi ? xi - 1 : xi;
    }
}
