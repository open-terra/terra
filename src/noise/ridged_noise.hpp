#pragma once

#include <cstdint>
#include <memory>

#include "../utils/fast_abs.hpp"

namespace Terra::Noise
{
    template<class T>
    class RidgedNoise
    {
    private:
        T& noise;

    public:
        constexpr RidgedNoise(int64_t seed) : noise(T(seed)) {}
        constexpr RidgedNoise(T& noise) : noise(noise) {}

        constexpr double Noise(double x, double y)
        {
            return 1.0 - Utils::FastAbs(noise.Noise(x, y));
        }
        constexpr double Noise(double x, double y, double z)
        {
            return 1.0 - Utils::FastAbs(noise.Noise(x, y, z));
        }
        constexpr double Noise(double x, double y, double z, double w)
        {
            return 1.0 - Utils::FastAbs(noise.Noise(x, y, z, w));
        }
    };
}