#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iterator>
#include <iostream>
#include <vector>

#include "../types/vector_2.hpp"

#include "../grid/hash_grid.hpp"
#include "../grid/poisson_disc_sampler.hpp"

int main(int argc, char* argv[])
{
	const int64_t size_x = 1000;
	const int64_t size_y = 500;
	const double radius = 10.0;

	std::vector<Terra::Vector2> points;
	Terra::Grid::HashGrid grid(size_x, size_y, radius);

    Terra::Grid::PoissonDiscSampler sampler(points, grid, size_x, size_y, radius, 100);
	sampler.Sample();

	std::cout << u8"Created " << points.size() << u8"points.";

	return 0;
}