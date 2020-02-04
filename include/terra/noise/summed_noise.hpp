#pragma once

#include <cstdint>
#include <memory>

#include "../base_types.hpp"

namespace terra::noise
{
    template<class T>
    class summed_noise
    {
    private:
        T& noise_source;
        size_t octaves;
        tfloat lunacracity;
        tfloat persistence;

    public:
        constexpr summed_noise(int64_t seed,
                               size_t octaves,
                               tfloat lunacracity,
                               tfloat persistence);
        constexpr summed_noise(T& noise,
                               size_t octaves,
                               tfloat lunacracity,
                               tfloat persistence);

        constexpr tfloat noise(tfloat x, tfloat y);
        constexpr tfloat noise(tfloat x, tfloat y, tfloat z);
        constexpr tfloat noise(tfloat x, tfloat y, tfloat z, tfloat w);
    };
} // namespace terra::noise
