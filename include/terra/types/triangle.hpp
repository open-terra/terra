#pragma once

#include <array>

namespace terra
{
    struct triangle
    {
    public:
        size_t v0, v1, v2;

        constexpr triangle() : v0(0), v1(0), v2(0)
        {
        }

        constexpr triangle(size_t v0, size_t v1, size_t v2) : v0(v0), v1(v1), v2(v2)
        {
        }

        constexpr std::array<size_t, 3> get_vert_idx() const
        {
            return {this->v0, this->v1, this->v2};
        }
    };
}