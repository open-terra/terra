#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iterator>
#include <iostream>
#include <vector>

#include "src/types/vector_2.hpp"

#include "src/generator/grid/hash_grid.hpp"
#include "src/generator/grid/poisson_disc_sampler.hpp"

int main(int argc, char* argv[])
{
	const int64_t size = 1024;
	const double radius = 2.5;//size / (2.0 * size);
	const double scale = 40960.0 / size; // scale of a pixel in metres

	std::vector<Terra::Vector2> points;
	Terra::Generator::Grid::HashGrid grid(size, size, radius);

    Terra::Generator::Grid::PoissonDiscSampler sampler(points, grid, size, size, radius, 100);
	sampler.Sample();

	std::cout << u8"Created " << points.size() << u8"points.";

	return 0;
}