#pragma once

#include <list>
#include <vector>

#include "base_types.hpp"
#include "types/dynarray.hpp"
#include "types/polygon.hpp"
#include "types/rect.hpp"
#include "types/vec2.hpp"

namespace terra
{
    class voronoi
    {
    public:
        voronoi();

        size_t num_cells() const;
        size_t num_edges() const;
        size_t num_vertices() const;

        void generate(const std::vector<terra::vec2>& points,
                      const terra::rect<tfloat>& bounds,
                      terra::dynarray<terra::polygon>& cells);

    private:
        size_t ncells, nedges, nverts;
    };
}