#include <benchmark/benchmark.h>

#include "terra/terra.hpp"

static void BM_fbm_noise(benchmark::State& state)
{
    terra::noise::fbm_noise noise(1337);
    noise.set_octaves(8);

    for (auto _ : state)
    {
        noise.noise(0, 0, 0, 512, 512, 1, 0.125);
    }
}
BENCHMARK(BM_fbm_noise);

static void BM_billowy_noise(benchmark::State& state)
{
    terra::noise::billowy_noise noise(1337);
    noise.set_octaves(8);

    for (auto _ : state)
    {
        noise.noise(0, 0, 0, 512, 512, 1, 0.125);
    }
}
BENCHMARK(BM_billowy_noise);

static void BM_ridged_noise(benchmark::State& state)
{
    terra::noise::ridged_noise noise(1337);
    noise.set_octaves(8);

    for (auto _ : state)
    {
        noise.noise(0, 0, 0, 512, 512, 1, 0.125);
    }
}
BENCHMARK(BM_ridged_noise);

static void BM_erosive_noise(benchmark::State& state)
{
    terra::noise::erosive_noise noise(1337);

    for (auto _ : state)
    {
        noise.noise(0, 0, 512, 512, 0.125, 8);
    }
}
BENCHMARK(BM_erosive_noise);

BENCHMARK_MAIN();
