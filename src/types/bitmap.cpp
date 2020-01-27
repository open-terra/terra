#include "terra/types/bitmap.hpp"

using namespace terra;

bitmap::bitmap() : width(0), height(0), num_pixels(0), raster()
{
}

bitmap::bitmap(uint32_t width, uint32_t height, uint32_t num_pixels, std::unique_ptr<argb_t[]>& raster) : 
    width(width), height(height), num_pixels(num_pixels), raster(std::move(raster))
{
}
