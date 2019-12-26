#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>

#include <catch2/catch.hpp>

#include "terra/poisson_disc_sampler.hpp"
#include "terra/types/hash_grid.hpp"
#include "terra/types/vec2.hpp"

TEST_CASE("can sample points correctly", "[poisson_disc_sampler]")
{
    constexpr auto width = 1000;
    constexpr auto height = 500;
    constexpr auto radius = 10.0;

    auto sampler = terra::poisson_disc_sampler(width, height, radius);

    REQUIRE(sampler.sample() > 2500);
}