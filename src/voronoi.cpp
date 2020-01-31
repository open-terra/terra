#include "terra/voronoi.hpp"

#include <algorithm>
#include <cstdint>
#include <limits>

#include "boost/polygon/voronoi.hpp"

template <>
struct boost::polygon::geometry_concept<terra::vec2> { typedef point_concept type; };

template <>
struct boost::polygon::point_traits<terra::vec2>
{
    typedef double coordinate_type;

    static inline coordinate_type get(const terra::vec2& point, orientation_2d orient)
    {
        return (orient == HORIZONTAL) ? point.x : point.y;
    }
};

typedef boost::polygon::voronoi_diagram<double> voronoi_diagram_t;

using namespace terra;

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
                const auto* v0 = edge->vertex0();
                vertices.push_back(bounds.clamp_vert({v0->x(), v0->y()}));
            }
            else
            {
                const auto& v0 = edge->vertex0();
                const auto& v1 = edge->vertex1();
                vertices.push_back(bounds.clamp_vert({v0->x(), v0->y()}));
                vertices.push_back(bounds.clamp_vert({v1->x(), v1->y()}));
            }
            edge = edge->next();
        } while (edge != cell.incident_edge());

        vertices.shrink_to_fit();

        cells[i] = {cell.source_index(), vertices};
        ++i;
    }
}
