#pragma once

#include <algorithm>

#include "vec2.hpp"

namespace terra
{
    template<typename T>
    struct rect
    {
    public:
        T x0, x1, y0, y1;

        constexpr rect() :
            x0(static_cast<T>(0)), x1(static_cast<T>(0)), y0(static_cast<T>(0)),
            y1(static_cast<T>(0))
        {
        }

        constexpr rect(T x0, T y0, T x1, T y1) : x0(x0), x1(x1), y0(y0), y1(y1)
        {
        }

        constexpr terra::vec2 clamp_vert(const vec2& p) const
        {
            return
            {
                std::clamp(p.x, this->x0, this->x1),
                std::clamp(p.y, this->y0, this->y1)
            };
        }

        constexpr bool within_extent(const vec2& p) const
        {
            return p.x >= this->x0 && p.x <= this->x1 && p.y >= this->y0 &&
                   p.y <= this->y1;
        }
    };
} // namespace terra
