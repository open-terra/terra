#pragma once

#include <limits>   

namespace Utils
{
    constexpr double _sqrtNewtonRaphson(double x, double curr, double prev)
    {
        return curr == prev
            ? curr
            : _sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
    }

    // Constant expresion square root implementation https://stackoverflow.com/questions/8622256/in-c11-is-sqrt-defined-as-constexpr
    constexpr double FastSqrt(double x)
    {
        return (x >= 0 && x < std::numeric_limits<double>::infinity())
            ? _sqrtNewtonRaphson(x, x, 0)
            : std::numeric_limits<double>::quiet_NaN();
    }
}