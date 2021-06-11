#pragma once

#include "../concepts.hpp"
#include "ceil.hpp"
#include "floor.hpp"

namespace terra::math
{
    template<Integral T, Real R>
    constexpr T round(R x)
    {
        return x > 0.0 ? floor<T>(x + 0.5) : ceil<T>(x - 0.5);
    }
} // namespace terra::math
