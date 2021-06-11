#pragma once

#include <array>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>
#include <glm/gtx/normal.hpp>

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
        static constexpr tfloat area(const terra::vec2& a,
                                     const terra::vec2& b,
                                     const terra::vec2& c)
        {
            tfloat det = (a.x * (b.y - c.y)) +
                         (b.x * (c.y - a.y)) +
                         (c.x * (a.y - b.y));

            return det / 2;
        }

        static constexpr bool point_within(const terra::vec2& p0,
                                           const terra::vec2& p1,
                                           const terra::vec2& p2,
                                           const terra::vec2& p)
        {
            /*
            * Function takes the point and the triangle's vertices. It finds the area
            * of  the passed triangle (p0 p1 p2), and then the areas of the three
            * triangles (p p1 p2), (p p0 p2), and (p p0 p1). If the sum of these three
            * is greater than the first, then the point is outside of the triangle.
            */

            tfloat total = terra::triangle::area(p0, p1, p2);
            tfloat a0 = terra::triangle::area(p, p1, p2);
            tfloat a1 = terra::triangle::area(p, p0, p2);
            tfloat a2 = terra::triangle::area(p, p0, p1);

            return !((a0 + a1 + a2) > total);
        }

        static inline tfloat spatial_interp(const terra::vec3& p0, const terra::vec3& p1, const terra::vec3& p2, const terra::vec2& p)
        {
            tfloat a = glm::distance(p, terra::make_vec2(p0));
            tfloat b = glm::distance(p, terra::make_vec2(p1));
            tfloat c = glm::distance(p, terra::make_vec2(p2));

            tfloat t = a + b + c;
            a /= t;
            b /= t;
            c /= t;

            return (p0.z * a) + (p1.z * b) + (p2.z * c);
        }

        static inline terra::vec3 normal(const terra::vec3& p0,
                                         const terra::vec3& p1,
                                         const terra::vec3& p2)
        {
            return glm::triangleNormal(p0, p1, p2);
        }
    };
}

constexpr bool operator==(const terra::triangle& l, const terra::triangle& r)
{
    return (l.v0 == r.v0) && (l.v1 == r.v1) && (l.v2 == r.v2);
}
