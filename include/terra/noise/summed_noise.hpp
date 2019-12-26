#pragma once

#include <cstdint>
#include <memory>

#include "../math/fast_abs.hpp"

namespace terra::noise
{
    template<class T>
    class summed_noise
    {
       private:
        T& noise_source;
        size_t octaves;
        double lunacracity;
        double persistence;

       public:
        constexpr summed_noise(int64_t seed,
                               size_t octaves,
                               double lunacracity,
                               double persistence);
        constexpr summed_noise(T& noise,
                               size_t octaves,
                               double lunacracity,
                               double persistence);

        constexpr double noise(double x, double y);
        constexpr double noise(double x, double y, double z);
        constexpr double noise(double x, double y, double z, double w);
    };
} // namespace terra::noise
