#pragma once

namespace terra::utils
{
    template <typename T>
    struct range_t
    {
        T b, e;
        range_t(T x, T y) : b(x), e(y) {}
        T begin()
        {
            return b;
        }
        T end()
        {
            return e;
        }
    };

    template <typename T>
    range_t<T> range(T b, T e)
    {
        return range_t<T>(b, e);
    }
}