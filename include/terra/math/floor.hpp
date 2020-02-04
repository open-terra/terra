#pragma once

#include "../base_types.hpp"

namespace terra::math
{
    template<typename T>
    constexpr T floor(tfloat x)
    {
        T xi = static_cast<T>(x);
        return x < xi ? xi - 1 : xi;
    }
} // namespace terra::math
