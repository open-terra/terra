#include "hash_grid.hpp"

using namespace Terra;

HashGrid::HashGrid()
{
    this->gridSizeX = -1;
    this->gridSizeY = -1;
    this->neighbours = 0;
    this->bucketSize = 0.0;
}

HashGrid::HashGrid(int64_t sizeX, int64_t sizeY, double radius)
{
    this->bucketSize = radius * sqrt1_2;
    this->gridSizeX = sizeX / bucketSize;
    this->gridSizeY = sizeY / bucketSize;
    this->neighbours = Utils::FastCeil<int64_t>(radius / this->bucketSize);

    const int64_t hashtableSize = this->gridSizeX * this->gridSizeY;
    this->hashtable = std::make_unique<int64_t[]>(hashtableSize);
    std::fill(hashtable.get(), hashtable.get() + hashtableSize, -1);
}

void HashGrid::Set(const Terra::Vector2 &point, int64_t index)
{
    int64_t x = Utils::FastFloor<int64_t>(point.x) / this->bucketSize;
    int64_t y = Utils::FastFloor<int64_t>(point.y) / this->bucketSize;

    int64_t hash = std::max<int64_t>(x * gridSizeX, 0) + std::max<int64_t>(y, 0);
    if(this->hashtable[hash] > -1)
    {
        throw double_index_error();
    }

    this->hashtable[hash] = index;
}

int64_t HashGrid::At(const Terra::Vector2 &point)
{
    int64_t x = Utils::FastFloor<int64_t>(point.x) / this->bucketSize;
    int64_t y = Utils::FastFloor<int64_t>(point.y) / this->bucketSize;

    return this->hashtable
    [
        std::max<int64_t>(x * gridSizeX, 0) +
        std::max<int64_t>(y, 0)
    ];
}

std::vector<int64_t> HashGrid::Neighbours(const Terra::Vector2& point)
{
    std::vector<int64_t> indexs;

    int64_t x = Utils::FastFloor<int64_t>(point.x) / this->bucketSize;
    int64_t y = Utils::FastFloor<int64_t>(point.y) / this->bucketSize;

    const int64_t x0 = std::max<int64_t>(x - neighbours, 0);
    const int64_t y0 = std::max<int64_t>(y - neighbours, 0);
    const int64_t x1 = std::min<int64_t>(x + neighbours + 1, this->gridSizeX);
    const int64_t y1 = std::min<int64_t>(y + neighbours + 1, this->gridSizeY);

    for (y = y0; y < y1; y++)
    {
        for (x = x0; x < x1; x++)
        {
            int64_t index = hashtable[(y * this->gridSizeX) + x];
            if (index > -1)
            {
                indexs.push_back(index);
            }
        }
    }

    return indexs;
}
