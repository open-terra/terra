#pragma once

#include <cstdint>
#include <vector>

namespace terra
{
    typedef uint8_t gray8;
    typedef uint16_t gray16;
    typedef uint32_t rgba;

    template<typename T>
    struct bitmap
    {
        uint32_t width, height, num_pixels;
        std::vector<T> raster;

        bitmap() : width(0), height(0), num_pixels(0), raster()
        {
        }

        bitmap(int width, int height, std::vector<T> raster) :
            width(width), height(height), num_pixels(width * height),
            raster(std::move(raster))
        {
        }
    };
}