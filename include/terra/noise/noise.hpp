#pragma once

#include "billowy_noise.hpp"
#include "open_simplex.hpp"
#include "ridged_noise.hpp"
#include "summed_noise.hpp"

namespace terra::noise
{
    typedef summed_noise<open_simplex> summed_open_simplex;
    typedef summed_noise<billowy_noise<open_simplex>> billowy_open_simplex;
    typedef summed_noise<ridged_noise<open_simplex>> ridged_open_simplex;
} // namespace terra::noise
