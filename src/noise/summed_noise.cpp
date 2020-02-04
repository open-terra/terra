#include "terra/noise/summed_noise.hpp"

using namespace terra::noise;

template<class T>
constexpr summed_noise<T>::summed_noise(int64_t seed,
                                        size_t octaves,
                                        tfloat lunacracity,
                                        tfloat persistence) :
    noise_source(T(seed)),
    octaves(octaves), lunacracity(lunacracity), persistence(persistence)
{
}

template<class T>
constexpr summed_noise<T>::summed_noise(T& noise_source,
                                        size_t octaves,
                                        tfloat lunacracity,
                                        tfloat persistence) :
    noise_source(noise),
    octaves(octaves), lunacracity(lunacracity), persistence(persistence)
{
}

template<class T>
constexpr tfloat summed_noise<T>::noise(tfloat x, tfloat y)
{
    tfloat frequency = lunacracity;
    tfloat amplitude = persistence;
    tfloat maxValue = amplitude; // Used for normalizing result to 0.0 - 1.0

    tfloat result = noise_source.noise(x, y) * amplitude;
    for (int i = 1; i < octaves; i++)
    {
        result += noise_source.noise(x * frequency, y * frequency) * amplitude;

        maxValue += amplitude;

        amplitude *= persistence;
        frequency *= lunacracity;
    }

    return result / maxValue;
}

template<class T>
constexpr tfloat summed_noise<T>::noise(tfloat x, tfloat y, tfloat z)
{
    tfloat frequency = lunacracity;
    tfloat amplitude = persistence;
    tfloat maxValue = amplitude; // Used for normalizing result to 0.0 - 1.0

    tfloat result = noise_source.noise(x, y, z) * amplitude;
    for (int i = 1; i < octaves; i++)
    {
        result +=
            noise_source.noise(x * frequency, y * frequency, z * frequency) *
            amplitude;

        maxValue += amplitude;

        amplitude *= persistence;
        frequency *= lunacracity;
    }

    return result / maxValue;
}

template<class T>
constexpr tfloat summed_noise<T>::noise(tfloat x, tfloat y, tfloat z, tfloat w)
{
    tfloat frequency = lunacracity;
    tfloat amplitude = persistence;
    tfloat maxValue = amplitude; // Used for normalizing result to 0.0 - 1.0

    tfloat result = noise_source.noise(x, y, z, w) * amplitude;
    for (int i = 1; i < octaves; i++)
    {
        result +=
            noise_source.noise(
                x * frequency, y * frequency, z * frequency, w * frequency) *
            amplitude;

        maxValue += amplitude;

        amplitude *= persistence;
        frequency *= lunacracity;
    }

    return result / maxValue;
}
