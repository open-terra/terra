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
        T& noise_source;

       public:
        constexpr ridged_noise(int64_t seed) : noise_source(T(seed))
        {
        }
        constexpr ridged_noise(T& noise) : noise_source(noise)
        {
        }

        constexpr double noise(double x, double y)
        {
            return 1.0 - terra::math::abs(noise_source.noise(x, y));
        }
        constexpr double noise(double x, double y, double z)
        {
            return 1.0 - terra::math::abs(noise_source.noise(x, y, z));
        }
        constexpr double noise(double x, double y, double z, double w)
        {
            return 1.0 - terra::math::abs(noise_source.noise(x, y, z, w));
        }
    };
} // namespace terra::noise