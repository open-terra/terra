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
struct boost::polygon::point_traits<terra::vec2>
{
    typedef double coordinate_type;

    static inline coordinate_type get(const terra::vec2& point,
                                      orientation_2d orient)
    {
        return (orient == HORIZONTAL) ? point.x : point.y;
    }
};

void clip_infinite_edge(const std::vector<terra::vec2>& points,
                        const terra::rect<double>& bounds,
                        const boost::polygon::voronoi_edge<double>* edge,
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

    double side = bounds.x1 - bounds.x0;
    double koef = side / std::max(terra::math::abs(direction.x), terra::math::abs(direction.y));
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

typedef boost::polygon::voronoi_diagram<double> voronoi_diagram_t;

using namespace terra;

voronoi::voronoi()
{
}

void voronoi::generate(const std::vector<terra::vec2>& points,
                       const terra::rect<double>& bounds,
                       terra::dynarray<terra::polygon>& cells)
{
    voronoi_diagram_t vd;
    construct_voronoi(points.begin(), points.end(), &vd);

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
