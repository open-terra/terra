#pragma once

#include "billowy_noise.hpp"
#include "open_simplex.hpp"
#include "ridged_noise.hpp"
#include "summed_noise.hpp"

namespace terra::noise
{
    #define SUMMED_OPEN_SIMPLEX(Type) summed_noise<Type<open_simplex>>

    typedef summed_noise<open_simplex> summed_open_simplex;
    typedef SUMMED_OPEN_SIMPLEX(billowy_noise) billowy_open_simplex;
    typedef SUMMED_OPEN_SIMPLEX(ridged_noise) ridged_open_simplex;
}
