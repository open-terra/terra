#pragma once

#include "../concepts.hpp"
#include "pi.hpp"

namespace terra::math
{
    template<Real R>
    static constexpr R deg_to_rad()
    {
        return PI() / static_cast<R>(180.0);
    }

    template<Real R>
    static constexpr R rad_to_deg()
    {
        return static_cast<R>(180.0) / PI()
    }

    template<Real R>
    constexpr R to_radian(R x)
    {
        return x * deg_to_rad();
    }

    template<Real R>
    constexpr R to_degrees(R x)
    {
        return x * rad_to_deg();
    }
} // namespace terra::math
