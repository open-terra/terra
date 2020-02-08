#include "terra/voronoi.hpp"

#include <algorithm>
#include <cstdint>

#include "boost/polygon/voronoi.hpp"

#include "terra/math/abs.hpp"

inline void clip_infinite_edge(const std::vector<terra::vec2>& points,
                               const terra::rect<tfloat>& bounds,
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

typedef boost::polygon::voronoi_diagram<double> diagram_t;
typedef boost::polygon::point_data<double> point_t;

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
    terra::dynarray<point_t> vpoints(points.size());
    for (size_t i = 0; i < points.size(); ++i)
    {
        const auto& p = points[i];
        vpoints[i] = {static_cast<double>(p.x), static_cast<double> (p.y)};
    }

    diagram_t vd;
    construct_voronoi(vpoints.begin(), vpoints.end(), &vd);

    this->ncells = vd.num_cells();
    this->nedges = vd.num_edges();
    this->nverts = vd.num_vertices();

    size_t i = 0;
    for (const auto& cell : vd.cells())
    {
        std::vector<terra::vec2> vertices;
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
                    vertices.push_back({ v0->x(), v0->y() });
                }
            }
            edge = edge->next();
        } while (edge != cell.incident_edge());

        cells[i] = terra::polygon(vertices);
        ++i;
    }
}
