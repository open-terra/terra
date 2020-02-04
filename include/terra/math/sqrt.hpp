#pragma once

#include <limits>

#include "../base_types.hpp"

namespace terra::math
{
    constexpr tfloat _sqrt_newton_raphson(tfloat x, tfloat curr, tfloat prev)
    {
        return curr == prev
                   ? curr
                   : _sqrt_newton_raphson(x, 0.5 * (curr + x / curr), curr);
    }

    // Constant expresion square root implementation
    // https://stackoverflow.com/questions/8622256/in-c11-is-sqrt-defined-as-constexpr
    constexpr tfloat sqrt(tfloat x)
    {
        return (x >= 0 && x < std::numeric_limits<tfloat>::infinity())
                   ? _sqrt_newton_raphson(x, x, 0)
                   : std::numeric_limits<tfloat>::quiet_NaN();
    }
} // namespace terra::math