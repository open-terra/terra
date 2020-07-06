#pragma once

#include <cstdint>
#include <memory>

#include "../base_types.hpp"
#include "../types/dynarray.hpp"

namespace terra::noise
{
    class erosive_noise
    {
    public:
        explicit erosive_noise(int32_t seed)
        {
        }

        terra::dynarray<tfloat> noise(size_t x,
                                      size_t y,
                                      size_t x_size,
                                      size_t y_size,
                                      tfloat scale,
                                      size_t octaves) const;
    };
} // namespace terra::noise
