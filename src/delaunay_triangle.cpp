#include "terra/delaunay.hpp"

#include "terra/types/triangle.hpp"

bool terra::delaunay::triangle::point_within(const terra::vec2& p)
{
    /* 
     * Function takes the point and the triangle's vertices. It finds the area
     * of  the passed triangle (p0 p1 p2), and then the areas of the three
     * triangles (p p1 p2), (p p0 p2), and (p p0 p1). If the sum of these three
     * is greater than the first, then the point is outside of the triangle.
     */

    const auto& p0 = *this->v0->v;
    const auto& p1 = *this->v1->v;
    const auto& p2 = *this->v2->v;

    tfloat total = terra::triangle::area(p0, p1, p2);
    tfloat a0 = terra::triangle::area(p, p1, p2);
    tfloat a1 = terra::triangle::area(p, p0, p2);
    tfloat a2 = terra::triangle::area(p, p0, p1);

    return !((a0 + a1 + a2) > total);
}
