#include "terra/voronoi.hpp"

#include <algorithm>
#include <cstdint>

#include "boost/polygon/voronoi.hpp"

#include "terra/math/abs.hpp"

typedef boost::polygon::voronoi_diagram<double> diagram_t;
typedef boost::polygon::voronoi_edge<double> edge_t;
typedef boost::polygon::point_data<double> point_t;

std::vector<terra::vec2> clip_infinite_edge(
    const std::vector<terra::vec2>& points,
    const terra::rect<tfloat>& bounds,
    const edge_t& edge)
{
    std::vector<terra::vec2> clipped_edge;
    clipped_edge.reserve(2);

    const auto& cell0 = *edge.cell();
    const auto& cell1 = *edge.twin()->cell();
    terra::vec2 origin, direction;

    const auto& p0 = points[cell0.source_index()];
    const auto& p1 = points[cell1.source_index()];
    origin.x = (p0.x + p1.x) * 0.5;
    origin.y = (p0.y + p1.y) * 0.5;
    direction.x = p0.y - p1.y;
    direction.y = p1.x - p0.x;

    double side = bounds.x1 - bounds.x0;
    double koef = side / std::max(terra::math::abs(direction.x), terra::math::abs(direction.y));
    if (edge.vertex0() == NULL)
    {
        clipped_edge.push_back(
            {
                origin.x - direction.x * koef,
                origin.y - direction.y * koef
            });
    }
    else
    {
        clipped_edge.push_back({edge.vertex0()->x(), edge.vertex0()->y()});
    }
    if (edge.vertex1() == NULL)
    {
        clipped_edge.push_back(
            {
                origin.x + direction.x * koef,
                origin.y + direction.y * koef
            });
    }
    else
    {
        clipped_edge.push_back({edge.vertex1()->x(), edge.vertex1()->y()});
    }
}

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
                if (edge->is_finite())
                {
                    // TODO avoid duplicates here
                    const auto* p0 = edge->vertex0();
                    vertices.push_back({p0->x(), p0->y()});
                    const auto* p1 = edge->vertex1();
                    vertices.push_back({p1->x(), p1->y()});
                }
                else
                {
                    auto verts = clip_infinite_edge(points, bounds, *edge);
                    for (auto& v : verts)
                    {
                        vertices.push_back(v);
                    }
                }
            }
            edge = edge->next();
        } while (edge != cell.incident_edge());

        cells[i] = terra::polygon(vertices);
        ++i;
    }
}
