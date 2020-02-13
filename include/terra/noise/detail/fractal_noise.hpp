#pragma once

#include <cstdint>
#include <memory>

#include "FastNoiseSIMD/FastNoiseSIMD.h"

#include "../../base_types.hpp"
#include "../../types/dynarray.hpp"

namespace terra::noise
{
    template<FastNoiseSIMD::FractalType noise_type>
    class fractal_noise_t
    {
    private:
        std::shared_ptr<FastNoiseSIMD> noise_source;

    public:
        explicit fractal_noise_t(int32_t seed) :
            noise_source(FastNoiseSIMD::NewFastNoiseSIMD(seed))
        {
        }

        fractal_noise_t(std::shared_ptr<FastNoiseSIMD>& noise_source) :
            noise_source(noise_source)
        {
        }

        terra::dynarray<tfloat> noise(size_t x,
                                      size_t y,
                                      size_t z,
                                      size_t x_size,
                                      size_t y_size,
                                      size_t z_size,
                                      tfloat scale) const
        {
            size_t size = x_size * y_size * z_size;
            terra::dynarray<tfloat> noise_set(size);

            noise_source->SetFractalType(noise_type);
            float* noise_temp = noise_source->GetSimplexFractalSet(x,
                                                                   y,
                                                                   z,
                                                                   x_size,
                                                                   y_size,
                                                                   z_size,
                                                                   scale);

            std::copy(noise_temp, noise_temp + size, noise_set.begin());

            noise_source->FreeNoiseSet(noise_temp);

            return noise_set;
        }
    };
} // namespace terra::noise
