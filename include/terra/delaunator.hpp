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
    class delaunator
    {
    public:
        delaunator();

        template<class ArrayVec> requires terra::Container<ArrayVec, terra::vec2>
        inline std::vector<size_t> triangulate(const ArrayVec& points);

    private:
        std::vector<size_t> _triangulate(const std::span<terra::vec2>& points);
    };
} // namespace terra

template<class ArrayVec> requires terra::Container<ArrayVec, terra::vec2>
inline std::vector<size_t> terra::delaunator::triangulate(const ArrayVec& points)
{
    return this->_triangulate(terra::to_span<terra::vec2>(points));
}
