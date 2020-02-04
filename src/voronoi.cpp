#include "terra/voronoi.hpp"

#include <algorithm>
#include <cstdint>

#include "boost/polygon/voronoi.hpp"

#include "terra/math/abs.hpp"

template<>
struct boost::polygon::geometry_concept<terra::vec2>
{
    typedef point_concept type;
};

template<>
struct boost::polygon::detail::ulp_comparison<float>
{
    enum Result
    {
        LESS = -1,
        EQUAL = 0,
        MORE = 1
    };

    Result operator()(float a, float b, unsigned int maxUlps) const
    {
        uint32_t ll_a, ll_b;

        // Reinterpret double bits as 32-bit signed integer.
        std::memcpy(&ll_a, &a, sizeof(float));
        std::memcpy(&ll_b, &b, sizeof(float));

        // Positive 0.0 is integer zero. Negative 0.0 is 0x80000000.
        // Map negative zero to an integer zero representation - making it
        // identical to positive zero - the smallest negative number is
        // represented by negative one, and downwards from there.
        if (ll_a < 0x80000000U)
            ll_a = 0x80000000U - ll_a;
        if (ll_b < 0x80000000U)
            ll_b = 0x80000000U - ll_b;

        // Compare 32-bit signed integer representations of input values.
        // Difference in 1 Ulp is equivalent to a relative error of between
        // 1/4,000,000,000,000,000 and 1/8,000,000,000,000,000.
        if (ll_a > ll_b)
            return (ll_a - ll_b <= maxUlps) ? EQUAL : LESS;
        return (ll_b - ll_a <= maxUlps) ? EQUAL : MORE;
    }
};

template<>
struct boost::polygon::point_traits<terra::vec2>
{
    typedef tfloat coordinate_type;

    static inline coordinate_type get(const terra::vec2& point,
                                      orientation_2d orient)
    {
        return (orient == HORIZONTAL) ? point.x : point.y;
    }
};

void clip_infinite_edge(const std::vector<terra::vec2>& points,
                        const terra::rect<tfloat>& bounds,
                        const boost::polygon::voronoi_edge<tfloat>* edge,
                        std::vector<terra::vec2>& clipped_edge)
{
    const auto* cell1 = edge->cell();
    const auto* cell2 = edge->twin()->cell();
    terra::vec2 origin, direction;
    // Infinite edges could not be created by two segment sites.
    if (cell1->contains_point() && cell2->contains_point())
    {
        auto& p1 = points[cell1->source_index()];
        auto& p2 = points[cell2->source_index()];
        origin.x = p1.x + p2.x * 0.5;
        origin.y = p1.y + p2.y * 0.5;
        direction.x = p1.y - p2.y;
        direction.y = p2.x - p1.x;
    }
    else
    {
        return;
    }

    tfloat side = bounds.x1 - bounds.x0;
    tfloat koef = side / std::max(terra::math::abs(direction.x), terra::math::abs(direction.y));
    if (edge->vertex0() == nullptr)
    {
        clipped_edge.push_back(
            {
                origin.x - direction.x * koef,
                origin.y - direction.y * koef
            });
    }
    else
    {
        clipped_edge.push_back({edge->vertex0()->x(), edge->vertex0()->y()});
    }

    if (edge->vertex1() == nullptr)
    {
        clipped_edge.push_back(
            {
                origin.x + direction.x * koef,
                origin.y + direction.y * koef
            });
    }
    else
    {
        clipped_edge.push_back({edge->vertex1()->x(), edge->vertex1()->y()});
    }
}

typedef boost::polygon::voronoi_diagram<tfloat> voronoi_diagram_t;

using namespace terra;

voronoi::voronoi() : ncells(0), nedges(0), nverts(0)
{
}

size_t voronoi::num_cells() const
{
    return this->ncells;
}

size_t voronoi::num_edges() const
{
    return this->nedges;
}

size_t voronoi::num_vertices() const
{
    return this->nverts;
}

void voronoi::generate(const std::vector<terra::vec2>& points,
                       const terra::rect<tfloat>& bounds,
                       terra::dynarray<terra::polygon>& cells)
{
    voronoi_diagram_t vd;
    construct_voronoi(points.begin(), points.end(), &vd);

    this->ncells = vd.num_cells();
    this->nedges = vd.num_edges();
    this->nverts = vd.num_vertices();

    size_t i = 0;
    for (const auto& cell : vd.cells())
    {
        std::vector<terra::vec2> vertices;
        vertices.reserve(6);
        const auto* edge = cell.incident_edge();
        // This is convenient way to iterate edges around Voronoi cell.
        do
        {
            if (edge->is_primary())
            {
                if(edge->is_infinite())
                {
                    clip_infinite_edge(points, bounds, edge, vertices);
                }
                else
                {
                    const auto& v0 = edge->vertex0();
                    vertices.push_back({v0->x(), v0->y()});
                }
            }
            edge = edge->next();
        } while (edge != cell.incident_edge());

        vertices.shrink_to_fit();

        cells[i] = {cell.source_index(), vertices};
        ++i;
    }
}
