#pragma once

#include "vec2.hpp"

namespace Terra
{
    template<typename T>
    struct Rect
    {
    public:
        T x0, x1, y0, y1;

        constexpr Rect() :
            x0(static_cast<T>(0)),
            x1(static_cast<T>(0)),
            y0(static_cast<T>(0)),
            y1(static_cast<T>(0))
        {
        }

        constexpr Rect(T x0, T y0, T x1, T y1) :
            x0(x0),
            x1(x1),
            y0(y0),
            y1(y1)
        {
        }

        constexpr bool WithinExtent(const vec2& p)
        {
            return p.x >= this->x0 && p.x <= this->x1 && p.y >= this->y0 && p.y <= this->y1;
        }
    };
}
