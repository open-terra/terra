#include "terra/types/hash_grid.hpp"

#include "terra/math/ceil.hpp"
#include "terra/math/floor.hpp"

using namespace terra;

hash_grid::hash_grid() :
    grid_size_x(-1), grid_size_y(-1), n(0), bucket_size(0.0), hashtable()
{
}

hash_grid::hash_grid(int64_t size_x, int64_t size_y, double radius)
{
    this->bucket_size = radius * hash_grid::sqrt1_2;
    this->grid_size_x =
        math::ceil<int64_t>(static_cast<double>(size_x) / bucket_size);
    this->grid_size_y =
        math::ceil<int64_t>(static_cast<double>(size_y) / bucket_size);
    this->n = math::ceil<int64_t>(radius / this->bucket_size);

    this->hashtable.reserve(this->grid_size_x * this->grid_size_y);
    std::fill(this->hashtable.begin(), this->hashtable.end(), -1);
}

constexpr size_t HashPos(const terra::vec2& point,
                         const double bucket_size,
                         const int64_t grid_size_x)
{
    int64_t x = std::max<int64_t>(math::floor<int64_t>(point.x) / bucket_size,
                                  0);
    int64_t y = std::max<int64_t>(math::floor<int64_t>(point.y) / bucket_size,
                                  0);

    return (y * grid_size_x) + x;
}

void hash_grid::set(const terra::vec2& point, int64_t index)
{
    int64_t hash = HashPos(point, this->bucket_size, this->grid_size_x);
    if (this->hashtable[hash] > -1)
    {
        throw double_index_error();
    }

    this->hashtable[hash] = index;
}

int64_t hash_grid::at(const terra::vec2& point)
{
    int64_t hash = HashPos(point, this->bucket_size, this->grid_size_x);
    return this->hashtable[hash];
}

std::vector<int64_t> hash_grid::neighbours(const terra::vec2& point)
{
    std::vector<int64_t> indexs;

    int64_t x =
        static_cast<int64_t>(math::floor<double>(point.x) / this->bucket_size);
    int64_t y =
        static_cast<int64_t>(math::floor<double>(point.y) / this->bucket_size);

    const int64_t x0 = std::max<int64_t>(x - this->n, 0);
    const int64_t y0 = std::max<int64_t>(y - this->n, 0);
    const int64_t x1 = std::min<int64_t>(x + this->n + 1, this->grid_size_x);
    const int64_t y1 = std::min<int64_t>(y + this->n + 1, this->grid_size_y);

    for (y = y0; y < y1; y++)
    {
        for (x = x0; x < x1; x++)
        {
            int64_t index = hashtable[(y * this->grid_size_x) + x];
            if (index > -1)
            {
                indexs.push_back(index);
            }
        }
    }

    return indexs;
}
