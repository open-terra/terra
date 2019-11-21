#pragma once

#include <cstdint>
#include <memory>

#include "../utils/fast_abs.hpp"

namespace terra::noise
{
    template<class T>
    class billowy_noise
    {
    private:
        T& noise;

    public:
        constexpr billowy_noise(int64_t seed) : noise(T(seed)) {}
        constexpr billowy_noise(T& noise) : noise(noise) {}

        constexpr double noise(double x, double y)
        {
            return utils::fast_abs(noise.noise(x, y));
        }
        constexpr double noise(double x, double y, double z)
        {
            return utils::fast_abs(noise.noise(x, y, z));
        }
        constexpr double noise(double x, double y, double z, double w)
        {
            return utils::fast_abs(noise.noise(x, y, z, w));
        }
    };
}
