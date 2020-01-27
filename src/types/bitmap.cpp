#include "terra/types/bitmap.hpp"

using namespace terra;

bitmap::bitmap() : width(0), height(0), num_pixels(0), raster()
{
}

bitmap::bitmap(uint32_t width, uint32_t height, uint32_t num_pixels, std::vector<argb_t>& raster) : 
    width(width), height(height), num_pixels(num_pixels), raster(std::move(raster))
{
}

std::vector<uint8_t> bitmap::get_channel(bitmap::channel c)
{
    std::vector<uint8_t> data;
    data.reserve(this->num_pixels);

    for(const auto& pixel : this->raster)
    {
        data.push_back(reinterpret_cast<const uint8_t*>(&pixel)[static_cast<size_t>(c)]);
    }

    return data;
}
