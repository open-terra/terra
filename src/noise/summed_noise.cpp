#include "summed_noise.hpp"

using namespace Terra::Noise;

template<class T>
constexpr SummedNoise<T>::SummedNoise(int64_t seed, size_t octaves, double lunacracity, double persistence) :
    noise(T(seed)),
    octaves(octaves),
    lunacracity(lunacracity),
    persistence(persistence),
{
}

template<class T>
constexpr SummedNoise<T>::SummedNoise(T& noise, size_t octaves, double lunacracity, double persistence) :
    noise(noise),
    octaves(octaves),
    lunacracity(lunacracity),
    persistence(persistence),
{
}

template<class T>
constexpr double SummedNoise<T>::Noise(double x, double y)
{
    double frequency = lunacracity;
    double amplitude = persistence;
    double maxValue = amplitude;  // Used for normalizing result to 0.0 - 1.0

    double result = noise.Noise(x, y) * amplitude;
    for(int i = 1; i < octaves; i++)
    {
        result += noise.Noise(x * frequency, y * frequency) * amplitude;
        
        maxValue += amplitude;
        
        amplitude *= persistence;
        frequency *= lunacracity;
    }

    return result / maxValue;
}

template<class T>
constexpr double SummedNoise<T>::Noise(double x, double y, double z)
{
    double frequency = lunacracity;
    double amplitude = persistence;
    double maxValue = amplitude;  // Used for normalizing result to 0.0 - 1.0

    double result = noise.Noise(x, y, z) * amplitude;
    for(int i = 1; i < octaves; i++)
    {
        result += noise.Noise(x * frequency, y * frequency, z * frequency) * amplitude;
        
        maxValue += amplitude;
        
        amplitude *= persistence;
        frequency *= lunacracity;
    }

    return result / maxValue;
}

template<class T>
constexpr double SummedNoise<T>::Noise(double x, double y, double z, double w)
{
    double frequency = lunacracity;
    double amplitude = persistence;
    double maxValue = amplitude;  // Used for normalizing result to 0.0 - 1.0

    double result = noise.Noise(x, y, z, w) * amplitude;
    for(int i = 1; i < octaves; i++)
    {
        result += noise.Noise(x * frequency, y * frequency, z * frequency, w * frequency) * amplitude;
        
        maxValue += amplitude;
        
        amplitude *= persistence;
        frequency *= lunacracity;
    }

    return result / maxValue;
}
