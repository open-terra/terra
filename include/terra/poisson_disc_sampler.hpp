#include <algorithm>
#include <cmath>
#include <cstdint>
#include <memory>
#include <random>
#include <vector>

#include "types/vector_2.hpp"
#include "types/rect.hpp"
#include "types/hash_grid.hpp"

namespace Terra
{
    class PoissonDiscSampler
    {
	private:
        const double pi = 3.14159265358979323846;
        const double sqrt1_2 = 0.7071067811865476;

        double radius;
        int64_t sizeX;
        int64_t sizeY;
        int64_t samples;

        double inner, outer;
        int64_t count;

        Terra::Rect<double> bounds;

        // Will be used to obtain a seed for the random number engine
        std::random_device rd;
        // Standard mersenne_twister_engine seeded with rd()
        std::mt19937 gen;
        // Random double between 0.0 and 1.0
        std::uniform_real_distribution<double> normal;

        std::vector<int64_t> active;

        std::vector<Terra::Vector2>* points;
		Terra::HashGrid* grid;

	public:
        PoissonDiscSampler();
        PoissonDiscSampler(std::vector<Terra::Vector2>& points, Terra::HashGrid& grid, int64_t sizeX, int64_t sizeY, double radius, int64_t samples = 30);

        int64_t Sample();

    private:
        const Terra::Vector2 GenerateAround(Terra::Vector2& p);
        bool IsValid(Terra::Vector2& p);
    };
}