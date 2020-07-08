#pragma once

#include <span>

#include "base_types.hpp"
#include "concepts.hpp"
#include "concepts_helpers.hpp"
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

        template<class ArrayVec, class ArrayPolygon>
        requires terra::Container<ArrayVec, terra::vec2>&&
                 terra::Container<ArrayPolygon, terra::polygon>
        inline void generate(const ArrayVec& points,
                             const terra::rect<tfloat>& bounds,
                             ArrayPolygon& cells);

    private:
        size_t ncells, nedges, nverts;

        void _generate(const std::span<const terra::vec2>& points,
                      const terra::rect<tfloat>& bounds,
                      std::span<terra::polygon>& cells);
    };
}

template<class ArrayVec, class ArrayPolygon>
requires terra::Container<ArrayVec, terra::vec2>&&
         terra::Container<ArrayPolygon, terra::polygon>
void terra::voronoi::generate(const ArrayVec& points,
                              const terra::rect<tfloat>& bounds,
                              ArrayPolygon& cells)
{
    this->_generate(terra::to_span<const terra::vec2>(points),
                    bounds,
                    terra::to_span<terra::polygon>(cells));
}
