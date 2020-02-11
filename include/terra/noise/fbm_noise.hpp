#pragma once

#include "FastNoiseSIMD/FastNoiseSIMD.h"

#include "detail/fractal_noise.hpp"

namespace terra::noise
{
    typedef fractal_noise_t<FastNoiseSIMD::FractalType::FBM> fbm_noise;
}
