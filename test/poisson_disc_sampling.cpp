#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iterator>
#include <iostream>
#include <vector>

#include <catch2/catch.hpp>

#include "terra/types/vec2.hpp"
#include "terra/types/hash_grid.hpp"
#include "terra/poisson_disc_sampler.hpp"

TEST_CASE("can sample points correctly", "[PoissonDiscSampler]")
{
	const int64_t size_x = 1000;
	const int64_t size_y = 500;
	const double radius = 10.0;

	std::vector<Terra::vec2> points;
	Terra::HashGrid grid(size_x, size_y, radius);

    Terra::PoissonDiscSampler sampler(points, grid, size_x, size_y, radius, 100);

	REQUIRE(sampler.Sample() >= 2500);
}