#pragma once

#include <cstdint>
#include <vector>

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
        enum struct channel : size_t
        {
            alpha = 0,
            red,
            green,
            blue
        };

        uint32_t width, height, num_pixels;
        std::vector<argb_t> raster;

        bitmap();
        bitmap(uint32_t width, uint32_t height, uint32_t num_pixels, std::vector<argb_t>& raster);

        std::vector<uint8_t> get_channel(channel c) const;
    };
}