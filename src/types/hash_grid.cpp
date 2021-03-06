#include "terra/types/hash_grid.hpp"

#include "terra/math.hpp"

terra::hash_grid::hash_grid() : m_cell_size(0.0), m_grid_width(0), m_grid_height(0), 
                                m_grid(0)
{
}

terra::hash_grid::hash_grid(size_t width, size_t height, tfloat radius) : 
    m_cell_size(radius / terra::math::sqrt(2.0)),
    m_grid_width(terra::math::ceil<size_t>(width / this->m_cell_size) + 1),
    m_grid_height(terra::math::ceil<size_t>(height / this->m_cell_size) + 1),
    m_grid(this->m_grid_width * this->m_grid_height)
{
    std::fill(this->m_grid.begin(), this->m_grid.end(), this->cell_empty);
}

void terra::hash_grid::set(const terra::vec2& p, size_t index)
{
    const size_t x = terra::math::floor<size_t>(p.x / this->m_cell_size);
    const size_t y = terra::math::floor<size_t>(p.y / this->m_cell_size);

    this->m_grid[y * this->m_grid_width + x] = index;
}

size_t terra::hash_grid::at(const terra::vec2& p) const
{
    const size_t x = terra::math::floor<size_t>(p.x / this->m_cell_size);
    const size_t y = terra::math::floor<size_t>(p.y / this->m_cell_size);

    return this->m_grid[y * this->m_grid_width + x];
}

bool terra::hash_grid::is_cell_empty(const terra::vec2& p) const
{
    return this->at(p) == this->cell_empty;
}

std::vector<size_t> terra::hash_grid::get_cells(size_t x_offset,
                                                size_t y_offset,
                                                size_t width,
                                                size_t height) const
{
    std::vector<size_t> cells;

    for (size_t y = y_offset; y <= y_offset + height; ++y)
    {
        const size_t offset = y * this->m_grid_width;
        for (size_t x = x_offset; x <= x_offset + width; ++x)
        {
            size_t i = this->m_grid[offset + x];
            if (i != this->cell_empty)
            {
                cells.push_back(i);
            }
        }
    }
    
    return cells;
}

std::vector<size_t> terra::hash_grid::get_neighbours(const terra::vec2& p,
                                                     size_t n) const
{
    std::vector<size_t> neighbours;

    const size_t x_index = math::floor<size_t>(p.x / this->m_cell_size);
    const size_t y_index = math::floor<size_t>(p.y / this->m_cell_size);

    const size_t min_x = std::max<size_t>(x_index - n, 0ull);
    const size_t min_y = std::max<size_t>(y_index - n, 0ull);
    const size_t max_x = std::min<size_t>(x_index + n, this->m_grid_width - 1);
    const size_t max_y = std::min<size_t>(y_index + n, this->m_grid_height - 1);

    for (size_t y = min_y; y <= max_y; ++y)
    {
        const size_t offset = y * this->m_grid_width;
        for (size_t x = min_x; x <= max_x; ++x)
        {
            if (x == x_index && y == y_index) continue;

            size_t i = this->m_grid[offset + x];
            if (i != this->cell_empty)
            {
                neighbours.push_back(i);
            }
        }
    }

    return neighbours;
}