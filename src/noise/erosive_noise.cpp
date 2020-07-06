#include "terra/noise/erosive_noise.hpp"

#include <algorithm>

#include "sdnoise1234.h"

float PERSISTANCE = 0.5;
float LUNACRACITY = 2.0;

// control the erosive noise
float ALPHA = 0.15; // feature displacement
float BETA = 1.1; // roughness near valeys

inline float get_noise(float x, float y, size_t octaves)
{
    float freq = 1.0;
    float amp = 1.0;
    float B = 0.0;
    float dx = 0.0;
    float dy = 0.0;
    float s = 1.0;

    for (size_t i = 0; i < octaves; ++i)
    {
        float pre_x = freq * (x + dx);
        float pre_y = freq * (y + dy);

        float gx, gy;
        float n = sdnoise2(pre_x, pre_y, &gx, &gy);
        float t_in = s * (1 - abs(n));
        B = B + (amp * t_in);

        // aproximate gradient because 1 - abs(N(p))
        float grad_x = -n * gx;
        float grad_y = -n * gy;

        dx = dx + (amp * ALPHA * s * grad_x);
        dy = dy + (amp * ALPHA * s * grad_y);
        s = s * std::min(1.0f, std::max(0.0f, BETA * B));

        amp = amp * PERSISTANCE;
        freq = freq * LUNACRACITY;
    }

    return B;
}

terra::dynarray<tfloat> terra::noise::erosive_noise::noise
(
    size_t x,
    size_t y,
    size_t x_size,
    size_t y_size,
    tfloat scale,
    size_t octaves
) const
{
    size_t size = x_size * y_size;
    terra::dynarray<tfloat> noise_set(size);

    size_t count = 0;
    for (size_t j = y; j < y + y_size; ++j)
    {
        for (size_t i = x; i < x + x_size; ++i)
        {
            float in_x = i * scale;
            float in_y = j * scale;

            noise_set[count] = get_noise(in_x, in_y, octaves);
        }
    }

    return noise_set;
}
