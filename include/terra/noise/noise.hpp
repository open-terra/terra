#pragma once

#include "billowy_noise.hpp"
#include "open_simplex.hpp"
#include "ridged_noise.hpp"
#include "summed_noise.hpp"

namespace Terra::Noise
{
    #define SUMMED_OPEN_SIMPLEX(Type) SummedNoise<Type<OpenSimplex>>

    typedef SummedNoise<OpenSimplex> SummedOpenSimplex;
    typedef SUMMED_OPEN_SIMPLEX(BillowyNoise) BillowyOpenSimplex;
    typedef SUMMED_OPEN_SIMPLEX(RidgedNoise) RidgedOpenSimplex;
}
