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

        /* Find the area of a triangle. This function uses the 1/2 determinant
         * method. Given three points (x1, y1), (x2, y2), (x3, y3):
         *             | x1 y1 1 |
         * Area = .5 * | x2 y2 1 |
         *             | x3 y3 1 |
         * From: http://mcraefamily.com/MathHelp/GeometryTriangleAreaDeterminant.htm
         */
        static constexpr tfloat area(const terra::vec2& a, const terra::vec2& b, const terra::vec2& c) const
        {
            tfloat det = (a.x * (b.y - c.y)) +
                         (b.x * (c.y - a.y)) +
                         (c.x * (a.y - b.y));

            return det / 2;
        }
    };
}