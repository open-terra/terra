#pragma once

#include <cstdint>
#include <memory>

#include "../math/abs.hpp"

namespace terra::noise
{
    template<class T>
    class billowy_noise
    {
       private:
        T& noise_source;

       public:
        constexpr billowy_noise(int64_t seed) : noise_source(T(seed))
        {
        }
        constexpr billowy_noise(T& noise) : noise_source(noise)
        {
        }

        constexpr double noise(double x, double y)
        {
            return terra::math::abs(noise_source.noise(x, y));
        }
        constexpr double noise(double x, double y, double z)
        {
            return terra::math::abs(noise_source.noise(x, y, z));
        }
        constexpr double noise(double x, double y, double z, double w)
        {
            return terra::math::abs(noise_source.noise(x, y, z, w));
        }
    };
} // namespace terra::noise
