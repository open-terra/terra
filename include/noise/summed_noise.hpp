#pragma once

#include <cstdint>
#include <memory>

#include "../utils/fast_abs.hpp"

namespace Terra::Noise
{
    template<class T>
    class SummedNoise
    {
    private:
        T& noise;
        size_t octaves;
        double lunacracity;
        double persistence;

    public:
        constexpr SummedNoise(int64_t seed, size_t octaves, double lunacracity, double persistence);
        constexpr SummedNoise(T& noise, size_t octaves, double lunacracity, double persistence);

        constexpr double Noise(double x, double y);
        constexpr double Noise(double x, double y, double z);
        constexpr double Noise(double x, double y, double z, double w);
    };
}
