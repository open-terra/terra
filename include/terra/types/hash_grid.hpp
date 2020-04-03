#pragma once

#include <cstdint>
#include <vector>

#include "../base_types.hpp"
#include "dynarray.hpp"
#include "rect.hpp"
#include "vec2.hpp"

namespace terra
{
    class hash_grid
    {
    public:
        constexpr static tfloat sqrt1_2 = static_cast<tfloat>(0.7071067811865476);
        constexpr static size_t cell_empty = std::numeric_limits<size_t>::max();

        hash_grid();
        hash_grid(size_t width, size_t height, tfloat radius);

        void set(const terra::vec2& p, size_t index);
        size_t at(const terra::vec2& p) const;

        bool is_cell_empty(const terra::vec2& p) const;

        inline tfloat get_cell_size() const;
        inline size_t get_grid_width() const;
        inline size_t get_grid_height() const;

        inline std::vector<size_t> get_cells(const terra::rect<size_t>& rect) const;
        std::vector<size_t> get_cells(size_t x_offset, size_t y_offset, size_t x, size_t y) const;
        std::vector<size_t> get_neighbours(const terra::vec2& p, const size_t n = 2) const;

    private:
        class double_index_error : public std::exception
        {
            virtual const char* what() const throw()
            {
                return "A value already exists at this location";
            }
        };

        tfloat m_cell_size;
        size_t m_grid_width, m_grid_height;
        terra::dynarray<size_t> m_grid;
    };
} // namespace terra

tfloat terra::hash_grid::get_cell_size() const
{
    return this->m_cell_size;
}

size_t terra::hash_grid::get_grid_width() const
{
    return this->m_grid_width;
}

size_t terra::hash_grid::get_grid_height() const
{
    return this->m_grid_height;
}

std::vector<size_t> terra::hash_grid::get_cells(const terra::rect<size_t>& rect) const
{
    return this->get_cells(rect.x0, rect.y0, rect.x1, rect.y1);
}
