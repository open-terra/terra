#pragma once

#include <cstdint>
#include <vector>

#include "../concepts.hpp"

namespace terra
{
    template<Vec2 PointConfig>
    class hash_grid
    {
    public:
        using config     = PointConfig;
        using point_type = typename PointConfig::point_type;
        using value_type = typename PointConfig::point_type::value_type;

        constexpr static value_type sqrt1_2 = static_cast<value_type>(0.7071067811865476);
        constexpr static size_t cell_empty = std::numeric_limits<size_t>::max();

        hash_grid();
        hash_grid(size_t width, size_t height, value_type radius);

        void resize(size_t width, size_t height, value_type radius);
        void clear();

        void set(const point_type& p, size_t index);
        size_t at(const point_type& p) const;

        bool is_cell_empty(const point_type& p) const;

        value_type get_cell_size() const;
        size_t get_grid_width() const;
        size_t get_grid_height() const;

        std::vector<size_t> get_cells(size_t x_offset, size_t y_offset, size_t x, size_t y) const;
        std::vector<size_t> get_neighbours(const point_type& p,
                                           const size_t n = 2) const;

    private:
        value_type m_cell_size;
        size_t m_grid_width, m_grid_height;
        std::vector<size_t> m_grid;
    };
} // namespace terra

#include "hash_grid.ipp"
