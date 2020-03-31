#pragma once

#include <cstdint>
#include <vector>

#include "../base_types.hpp"
#include "dynarray.hpp"
#include "vec2.hpp"

namespace terra
{
    class hash_grid
    {
        class double_index_error : public std::exception
        {
            virtual const char* what() const throw()
            {
                return "A value already exists at this location";
            }
        };

        const tfloat sqrt1_2 = 0.7071067811865476;

        tfloat cell_size;
        size_t grid_width, grid_height;

        terra::dynarray<size_t> grid;

    public:
        constexpr static size_t cell_empty = std::numeric_limits<size_t>::max();

        hash_grid();
        hash_grid(size_t width, size_t height, tfloat radius);

        void set(const terra::vec2& p, size_t index);
        size_t at(const terra::vec2& p) const;

        bool is_cell_empty(const terra::vec2& p) const;

        std::vector<size_t> get_neighbours(const terra::vec2& p, const size_t n = 2) const;
    };
} // namespace terra
