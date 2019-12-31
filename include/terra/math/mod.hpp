#pragma once

namespace terra::math
{
    //@see
    // https://stackoverflow.com/questions/33333363/built-in-mod-vs-custom-mod-function-improve-the-performance-of-modulus-op/33333636#33333636
    template<typename T>
    constexpr T mod(const T i, const T c)
    {
        return i >= c ? i % c : i;
    }
} // namespace terra::math