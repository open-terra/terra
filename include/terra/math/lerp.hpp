#pragma once

#include "../base_types.hpp"

namespace terra::math
{
    constexpr tfloat lerp(tfloat a, tfloat b, tfloat w)
    {
        return (a * (static_cast<tfloat>(1) - w)) + (b * w);
    }
} // namespace terra::math
