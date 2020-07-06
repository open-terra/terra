#include "terra/uplift.hpp"

#include <limits>

#include "terra/math/floor.hpp"

using namespace terra;

bitmap_uplift::bitmap_uplift(const terra::bitmap& bitmap) : 
    width(bitmap.get_width()), raster(bitmap.size())
{
    const uint8_t* raster_view = bitmap.get();
    std::copy(raster_view, raster_view + bitmap.size(), raster.data());
}

tfloat bitmap_uplift::at(const terra::vec2& p) const
{
    // TODO probably use something other than a floor here.
    const size_t px = math::floor<size_t>(p.x);
    const size_t py = math::floor<size_t>(p.y);
    const uint8_t val = this->raster[(py * width) + px];
    return static_cast<tfloat>(val) / std::numeric_limits<uint8_t>::max();
}

tfloat terra::noise_uplift::at(const terra::vec2& p) const
{
    // TODO probably use something other than a floor here.
    auto point = p * this->scale;
    const size_t px = std::min(math::floor<size_t>(point.x), this->width - 1);
    const size_t py = std::min(math::floor<size_t>(point.y), this->height - 1);
    return this->noise_map[(py * this->width) + px];
}

terra::linear_uplift::linear_uplift(size_t width,
                                    size_t height,
                                    tfloat min,
                                    tfloat max,
                                    orient orientation) :
    width(width), height(height), min(min), diff(max - min),
    orientation(orientation)
{
}

tfloat terra::linear_uplift::at(const terra::vec2& p) const
{
    tfloat uplift = this->min;
    switch(this->orientation)
    {
        case terra::linear_uplift::orient::north:
            uplift += this->diff * (p.y / height);
            break;
        case terra::linear_uplift::orient::south:
            uplift += this->diff * (1.0f - (p.y / height));
            break;
        case terra::linear_uplift::orient::east:
            uplift += this->diff * (p.x / width);
            break;
        case terra::linear_uplift::orient::west:
            uplift += this->diff * (1.0f - (p.x / width));
            break;
    }

    return uplift;
}

uplift::uplift() : factor(0.0f), uplifts(0), points(nullptr), heights(nullptr)
{
}

void uplift::update()
{
    for (size_t i = 0; i < points->size(); ++i)
    {
        this->heights->data()[i] += factor * this->uplifts[i];
    }
}
