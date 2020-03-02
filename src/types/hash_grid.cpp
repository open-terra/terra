#include "terra/types/hash_grid.hpp"

#include "terra/math.hpp"

terra::hash_grid::hash_grid() : cell_size(0.0), grid_width(0), grid_height(0), 
                                grid(0)
{
}

terra::hash_grid::hash_grid(size_t width, size_t height, tfloat radius) : 
    cell_size(radius / terra::math::sqrt(2.0)),
    grid_width(terra::math::ceil<size_t>(width / this->cell_size) + 1),
    grid_height(terra::math::ceil<size_t>(height / this->cell_size) + 1),
    grid(this->grid_width * this->grid_height)
{
}

void terra::hash_grid::set(const terra::vec2& p, size_t index)
{
    const size_t x = terra::math::floor<size_t>(p.x / this->cell_size);
    const size_t y = terra::math::floor<size_t>(p.y / this->cell_size);

    this->grid[y * this->grid_width + x] = index;
}

size_t terra::hash_grid::at(const terra::vec2& p) const
{
    const size_t x = terra::math::floor<size_t>(p.x / this->cell_size);
    const size_t y = terra::math::floor<size_t>(p.y / this->cell_size);

    return this->grid[y * this->grid_width + x];
}

bool terra::hash_grid::is_cell_empty(const terra::vec2& p) const
{
    return this->at(p) == this->cell_empty;
}

std::vector<size_t> terra::hash_grid::get_neighbours(const terra::vec2& p) const
{
    std::vector<size_t> neighbours;

    const size_t x_index = math::floor<size_t>(p.x / this->cell_size);
    const size_t y_index = math::floor<size_t>(p.y / this->cell_size);

    const size_t min_x = std::max<size_t>(x_index - 2, 0ull);
    const size_t min_y = std::max<size_t>(y_index - 2, 0ull);
    const size_t max_x = std::min<size_t>(x_index + 2, this->grid_width - 1);
    const size_t max_y = std::min<size_t>(y_index + 2, this->grid_height - 1);

    for (size_t y = min_y; y <= max_y; ++y)
    {
        const size_t offset = y * this->grid_width;
        for (size_t x = min_x; x <= max_x; ++x)
        {
            size_t i = this->grid[offset + x];
            if (i != this->cell_empty)
            {
                neighbours.push_back(i);
            }
        }
    }

    return neighbours;
}