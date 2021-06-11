#include "terra/types/hash_grid.hpp"

#include "terra/math.hpp"

template<terra::Vec2 PointConfig>
inline terra::hash_grid<PointConfig>::hash_grid()
    : m_cell_size(0.0), m_grid_width(0), m_grid_height(0), m_grid(0)
{
}

template<terra::Vec2 PointConfig>
inline terra::hash_grid<PointConfig>::hash_grid(size_t width, size_t height, value_type radius)
    : m_cell_size(radius / terra::math::sqrt(2.0)),
      m_grid_width(terra::math::ceil<size_t>(width / this->m_cell_size) + 1),
      m_grid_height(terra::math::ceil<size_t>(height / this->m_cell_size) + 1),
      m_grid(this->m_grid_width * this->m_grid_height)
{
    std::fill(this->m_grid.begin(), this->m_grid.end(), this->cell_empty);
}

template<terra::Vec2 PointConfig>
inline void terra::hash_grid<PointConfig>::resize(size_t width, size_t height, value_type radius)
{
    m_cell_size   = radius / terra::math::sqrt(2.0);
    m_grid_width  = terra::math::ceil<size_t>(width / this->m_cell_size) + 1;
    m_grid_height = terra::math::ceil<size_t>(height / this->m_cell_size) + 1;

    m_grid.resize(m_grid_width * m_grid_height);
    std::fill(this->m_grid.begin(), this->m_grid.end(), this->cell_empty);
}

template<terra::Vec2 PointConfig>
void terra::hash_grid<PointConfig>::set(const point_type& p, size_t index)
{
    const size_t x = terra::math::floor<size_t>(p.x / this->m_cell_size);
    const size_t y = terra::math::floor<size_t>(p.y / this->m_cell_size);

    this->m_grid[y * this->m_grid_width + x] = index;
}

template<terra::Vec2 PointConfig>
size_t terra::hash_grid<PointConfig>::at(const point_type& p) const
{
    const size_t x = terra::math::floor<size_t>(p.x / this->m_cell_size);
    const size_t y = terra::math::floor<size_t>(p.y / this->m_cell_size);

    return this->m_grid[y * this->m_grid_width + x];
}

template<terra::Vec2 PointConfig>
bool terra::hash_grid<PointConfig>::is_cell_empty(const point_type& p) const
{
    return this->at(p) == this->cell_empty;
}

template<terra::Vec2 PointConfig>
typename terra::hash_grid<PointConfig>::value_type terra::hash_grid<PointConfig>::get_cell_size() const
{
    return this->m_cell_size;
}

template<terra::Vec2 PointConfig>
size_t terra::hash_grid<PointConfig>::get_grid_width() const
{
    return this->m_grid_width;
}

template<terra::Vec2 PointConfig>
size_t terra::hash_grid<PointConfig>::get_grid_height() const
{
    return this->m_grid_height;
}

template<terra::Vec2 PointConfig>
std::vector<size_t> terra::hash_grid<PointConfig>::get_cells(size_t x_offset,
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

template<terra::Vec2 PointConfig>
std::vector<size_t> terra::hash_grid<PointConfig>::get_neighbours(const point_type& p,
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