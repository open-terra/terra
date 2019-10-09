#include <algorithm>
#include <cmath>
#include <cstdint>
#include <memory>
#include <random>
#include <vector>

#include "Types/Vector2.hpp"
#include "Types/HashGrid.hpp"

namespace Terra::Generator::Grid
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

        std::vector<Terra::Vector2>* points;
		Terra::HashGrid* grid;

	public:
        PoissonDiscSampler();

        PoissonDiscSampler(std::vector<Terra::Vector2>& points, Terra::HashGrid& grid, int64_t sizeX, int64_t sizeY, double radius, int64_t samples = 30);

        int64_t Sample();
    };
}