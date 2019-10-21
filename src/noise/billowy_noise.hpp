#pragma once

#include <cstdint>
#include <memory>

#include "../utils/fast_abs.hpp"

namespace Terra::Noise
{
    template<class T>
    class BillowyNoise
    {
    private:
        T& noise;

    public:
        constexpr BillowyNoise(int64_t seed) : noise(T(seed)) {}
        constexpr BillowyNoise(T& noise) : noise(noise) {}

        constexpr double Noise(double x, double y)
        {
            return Utils::FastAbs(noise.Noise(x, y));
        }
        constexpr double Noise(double x, double y, double z)
        {
            return Utils::FastAbs(noise.Noise(x, y, z));
        }
        constexpr double Noise(double x, double y, double z, double w)
        {
            return Utils::FastAbs(noise.Noise(x, y, z, w));
        }
    };
}
