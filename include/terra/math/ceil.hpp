#pragma once

#include "../concepts.hpp"

namespace terra::math
{
    template<Integral T, Real R>
    constexpr T ceil(R x)
    {
        T xi = static_cast<T>(x);
        return x > xi ? xi + 1 : xi;
    }
} // namespace terra::math
