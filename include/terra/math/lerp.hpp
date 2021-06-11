#pragma once

#include "../concepts.hpp"

namespace terra::math
{
    template<Real R>
    constexpr R lerp(R a, R b, R w)
    {
        return (a * (static_cast<R>(1) - w)) + (b * w);
    }
} // namespace terra::math
