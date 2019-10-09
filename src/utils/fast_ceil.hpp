#pragma once

namespace Utils
{
    template <typename T>
    inline T FastCeil(double x)
    {
        T xi = static_cast<T>(x);
        return x > xi ? xi + 1 : xi;
    }
}
