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
	const int64_t size = 1024;
	const double radius = 2.5;//size / (2.0 * size);
	const double scale = 40960.0 / size; // scale of a pixel in metres

	std::vector<Terra::Vector2> points;
	Terra::Grid::HashGrid grid(size, size, radius);

    Terra::Grid::PoissonDiscSampler sampler(points, grid, size, size, radius, 100);
	sampler.Sample();

	std::cout << u8"Created " << points.size() << u8"points.";

	return 0;
}