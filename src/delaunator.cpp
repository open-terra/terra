#include "terra/delaunator.hpp"

#define DELAUNATOR_HEADER_ONLY
#ifndef TERRA_USE_DOUBLE
#   define DELAUNATOR_USE_SINGLE
#endif
#include "delaunator.hpp"

class TerraPointConfig
{
public:
    using point_type = terra::vec2;

    static inline dfloat get_x(const point_type& p);
    static inline dfloat get_y(const point_type& p);
    static inline dfloat get_magnitude2(const point_type& p);
    static inline dfloat get_determinant(const point_type& p0,
                                         const point_type& p1);
    static inline point_type get_vector(const point_type& p0,
                                        const point_type& p1);
    static inline dfloat get_dist2(const point_type& p0,
                                   const point_type& p1);
    static inline bool get_equal(const point_type& p0,
                                 const point_type& p1, dfloat span);
};

typedef delaunator::Delaunator<TerraPointConfig> triangulator_t;

terra::delaunator::delaunator()
{
}

std::vector<size_t> terra::delaunator::_triangulate(const std::span<terra::vec2>& points)
{
    triangulator_t triangulator(points);
    return triangulator.triangles;
}

dfloat TerraPointConfig::get_x(const terra::vec2& p)
{
    return p.x;
}

dfloat TerraPointConfig::get_y(const terra::vec2& p)
{
    return p.y;
}

dfloat TerraPointConfig::get_magnitude2(const terra::vec2& p)
{
    return (p.x * p.x) + (p.y * p.y);
}

dfloat TerraPointConfig::get_determinant(const terra::vec2& p0,
                                         const terra::vec2& p1)
{
    return (p0.x * p1.y) - (p0.y * p1.x);
}

terra::vec2 TerraPointConfig::get_vector(const terra::vec2& p0,
                                         const terra::vec2& p1)
{
    return terra::vec2(p1.x - p0.x, p1.y - p0.y);
}

dfloat TerraPointConfig::get_dist2(const terra::vec2& p0, const terra::vec2& p1)
{
    const terra::vec2 vec = TerraPointConfig::get_vector(p0, p1);
    return (vec.x * vec.x) + (vec.y * vec.y);
}

bool TerraPointConfig::get_equal(const terra::vec2& p0,
                                 const terra::vec2& p1,
                                 dfloat span)
{
    dfloat dist = TerraPointConfig::get_dist2(p0, p1) / span;

    // ABELL - This number should be examined to figure how how
    // it correlates with the breakdown of calculating determinants.
    return dist < 1e-20;
}