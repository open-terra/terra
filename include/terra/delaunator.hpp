#pragma once

#include <limits>
#include <span>
#include <vector>

#include "base_types.hpp"
#include "concepts.hpp"
#include "concepts_helpers.hpp"
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
        std::span<const terra::vec2> coords;
        std::vector<size_t> hull_prev;
        std::vector<size_t> hull_next;
        std::vector<size_t> hull_tri;
        std::vector<size_t> hash;
        terra::vec2 centre;
        size_t hash_size;
        std::vector<size_t> edge_stack;

    public:
        delaunator();

        template<class T> requires terra::Container<T, terra::vec2>
        inline void triangulate(const T& points);
        tfloat get_hull_area();

    private:
        void triangulate(const std::span<const terra::vec2>& points);
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

template<class T> requires terra::Container<T, terra::vec2>
inline void terra::delaunator::triangulate(const T& points)
{
    this->triangulate(terra::to_span<const terra::vec2>(points));
}
