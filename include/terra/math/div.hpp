#pragma once

namespace terra::math
{
    template<typename T>
    struct div_t
    {
        T quot;
        T rem;
    };

    template<typename T = size_t, typename U = size_t>
    constexpr div_t<U> div(T x, T y)
    {
        const U quot = x / y;
        const U rem = x % y;
        return {quot, rem};
    }
} // namespace terra::math
