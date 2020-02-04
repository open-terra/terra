#pragma once

#include <cstdint>
#include <memory>

#include "../base_types.hpp"
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

        constexpr tfloat noise(tfloat x, tfloat y)
        {
            return terra::math::abs(noise_source.noise(x, y));
        }
        constexpr tfloat noise(tfloat x, tfloat y, tfloat z)
        {
            return terra::math::abs(noise_source.noise(x, y, z));
        }
        constexpr tfloat noise(tfloat x, tfloat y, tfloat z, tfloat w)
        {
            return terra::math::abs(noise_source.noise(x, y, z, w));
        }
    };
} // namespace terra::noise
