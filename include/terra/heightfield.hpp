#pragma once

#include <limits>
#include <span>

#include "concepts.hpp"
#include "types/bitmap.hpp"
#include "types/dynarray.hpp"

namespace terra
{
    class heightfield
    {
    public:
        inline heightfield();

        template<class T, class U> requires terra::Container<U, T>
        inline terra::bitmap raster(size_t x,
                                    size_t y,
                                    T min,
                                    T max,
                                    const U& data);
    };
}

terra::heightfield::heightfield()
{
}

template<class T, class U> requires terra::Container<U, T>
terra::bitmap terra::heightfield::raster(size_t x,
                                         size_t y,
                                         T min, 
                                         T max,
                                         const U& data)
{
    const T diff = max - min;

    terra::dynarray<uint8_t> raster(data.size());
    for (size_t i = 0; i < data.size(); ++i)
    {
        const auto ci = static_cast<size_t>(((data[i] - min) / diff) * 255.0);
        raster[i] = std::clamp<size_t>(ci, 0ull, 255ull);
    }

    return terra::bitmap(x, y, 8, 1, x * y, raster);
}
