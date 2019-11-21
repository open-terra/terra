#include <algorithm>
#include <cmath>
#include <cstdint>
#include <memory>
#include <random>
#include <vector>

#include "types/vec2.hpp"

#include "types/rect.hpp"
#include "types/hash_grid.hpp"

namespace terra
{
    class poisson_disc_sampler
    {
	private:
        const double pi = 3.14159265358979323846;

        double radius;
        int64_t size_x;
        int64_t size_y;
        int64_t samples;

        double inner, outer;
        int64_t count;

        terra::rect<double> bounds;

        // Will be used to obtain a seed for the random number engine
        std::random_device rd;
        // Standard mersenne_twister_engine seeded with rd()
        std::mt19937 gen;
        // Random double between 0.0 and 1.0
        std::uniform_real_distribution<double> normal;

        std::vector<int64_t> active;

        std::vector<terra::vec2>* points;
		terra::hash_grid* grid;

	public:
        poisson_disc_sampler();
        poisson_disc_sampler(std::vector<terra::vec2>& points, terra::hash_grid& grid, int64_t size_x, int64_t size_y, double radius, int64_t samples = 30);

        int64_t sample();

    private:
        const terra::vec2 generate_around(terra::vec2& p);
        bool is_valid(terra::vec2& p);
    };
}