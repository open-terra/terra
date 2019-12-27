#pragma once

#include <cstdint>
#include <memory>

#include "../math/abs.hpp"

namespace terra::noise
{
    template<class T>
    class ridged_noise
    {
       private:
        T& noise;

       public:
        constexpr ridged_noise(int64_t seed) : noise(T(seed))
        {
        }
        constexpr ridged_noise(T& noise) : noise(noise)
        {
        }

        constexpr double noise(double x, double y)
        {
            return 1.0 - terra::math::abs(noise.noise(x, y));
        }
        constexpr double noise(double x, double y, double z)
        {
            return 1.0 - terra::math::abs(noise.noise(x, y, z));
        }
        constexpr double noise(double x, double y, double z, double w)
        {
            return 1.0 - terra::math::abs(noise.noise(x, y, z, w));
        }
    };
} // namespace terra::noise