#pragma once

#include <cstdint>
#include <memory>

#include "../utils/template_helpers.hpp"

namespace terra
{
    struct argb_t
    {
    public:
        uint8_t alpha, red, green, blue;
    };

    struct bitmap
    {
    public:
        uint32_t width, height, num_pixels;
        std::unique_ptr<argb_t[]> raster;

        bitmap();
        bitmap(uint32_t width, uint32_t height, uint32_t num_pixels, std::unique_ptr<argb_t[]>& raster);
    };
}