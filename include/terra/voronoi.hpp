#pragma once

#include <list>
#include <vector>

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

        void generate(const std::vector<terra::vec2>& points,
                      const terra::rect<double>& bounds,
                      terra::dynarray<terra::polygon>& cells);
    };
}