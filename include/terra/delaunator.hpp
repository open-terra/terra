#pragma once

#include <limits>
#include <span>
#include <vector>

#include "base_types.hpp"
#include "types/vec2.hpp"

namespace terra
{
    constexpr tfloat EPSILON = std::numeric_limits<tfloat>::epsilon();
    constexpr size_t INVALID_INDEX = std::numeric_limits<size_t>::max();

    class delaunator
    {
    public:
        std::vector<size_t> triangles;
        std::vector<size_t> halfedges;
        size_t hull_start;
    private:
        const std::span<const terra::vec2>* coords;
        std::vector<size_t> hull_prev;
        std::vector<size_t> hull_next;
        std::vector<size_t> hull_tri;
        std::vector<size_t> hash;
        terra::vec2 centre;
        size_t hash_size;
        std::vector<size_t> edge_stack;

    public:
        delaunator();

        void triangulate(const std::span<const terra::vec2>& in_coords);
        tfloat get_hull_area();

    private:
        size_t legalize(size_t a);
        size_t hash_key(const terra::vec2& vec) const;
        size_t add_triangle(size_t i0,
                            size_t i1,
                            size_t i2,
                            size_t a,
                            size_t b,
                            size_t c);
        void link(size_t a, size_t b);
    };
} // namespace terra
