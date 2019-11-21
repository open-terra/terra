#pragma once

#include <limits>   

namespace utils
{
    constexpr double _sqrt_newton_raphson(double x, double curr, double prev)
    {
        return curr == prev
            ? curr
            : _sqrt_newton_raphson(x, 0.5 * (curr + x / curr), curr);
    }

    // Constant expresion square root implementation https://stackoverflow.com/questions/8622256/in-c11-is-sqrt-defined-as-constexpr
    constexpr double fast_sqrt(double x)
    {
        return (x >= 0 && x < std::numeric_limits<double>::infinity())
            ? _sqrt_newton_raphson(x, x, 0)
            : std::numeric_limits<double>::quiet_NaN();
    }
}