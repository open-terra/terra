#pragma once

#include <limits>

#include "../concepts.hpp"

namespace terra::math
{
    template<Real R>
    constexpr R _sqrt_newton_raphson(R x, R curr, R prev)
    {
        return curr == prev
                   ? curr
                   : _sqrt_newton_raphson(
                         x, static_cast<R>(0.5) * (curr + x / curr), curr);
    }

    // Constant expresion square root implementation
    // https://stackoverflow.com/questions/8622256/in-c11-is-sqrt-defined-as-constexpr
    template<Real R>
    constexpr R sqrt(R x)
    {
        return (x >= 0 && x < std::numeric_limits<R>::infinity())
                   ? _sqrt_newton_raphson(x, x, 0)
                   : std::numeric_limits<R>::quiet_NaN();
    }
} // namespace terra::math