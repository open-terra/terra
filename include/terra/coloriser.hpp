#pragma once

#include <limits>

#include "types/bitmap.hpp"
#include "types/dynarray.hpp"

namespace terra
{
    class coloriser
    {
    public:
        coloriser();

        template<class T>
        terra::bitmap raster(size_t x, size_t y,
                             T min, T max,
                             const terra::dynarray<T>& data,
                             const terra::rgb_t* colors);
    };
}

terra::coloriser::coloriser()
{
}

template<class T>
terra::bitmap terra::coloriser::raster(size_t x, size_t y,
                                       T min, T max,
                                       const terra::dynarray<T>& data,
                                       const terra::rgb_t* colors)
{
    const T diff = max - min;

    terra::dynarray<uint8_t> raster(data.size() * 3);
    for (size_t i = 0; i < data.size(); ++i)
    {
        const auto ci = static_cast<size_t>(((data[i] - min) / diff) * 255.0);

        const auto index = i * 3;
        const auto& color = colors[std::clamp<size_t>(ci, 0ull, 255ull)];
        raster[index]     = color.red;
        raster[index + 1] = color.green;
        raster[index + 2] = color.blue;
    }

    return terra::bitmap(x, y, 8, 3, x * y, raster);
}
