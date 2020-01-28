#include "terra/uplift.hpp"

#include "terra/math/floor.hpp"

using namespace terra;

uplift::uplift() :
    uplifts(), uplift_map(nullptr), points(nullptr), heights(nullptr)
{
}

uplift::uplift(const terra::bitmap& uplift_map,
               const std::vector<terra::vec2>& points,
               std::vector<double>& heights) :
    uplifts(), uplift_map(&uplift_map), points(&points), heights(&heights)
{
    const size_t bitmap_width = uplift_map.width;
    std::vector<uint8_t> uplift = uplift_map.get_channel(terra::bitmap::channel::alpha);

    uplifts.reserve(points.size());
    for (const auto& p : points)
    {
        // TODO probably use something other than a simple point sample here.
        const size_t px = math::floor<size_t>(p.x);
        const size_t py = math::floor<size_t>(p.y);
        uplifts.push_back(uplift[(py * bitmap_width) + px]);
    }
}

void uplift::update()
{
    for (size_t i = 0; i < points->size(); ++i)
    {
        (*this->heights)[i] += this->uplifts[i];
    }
}
