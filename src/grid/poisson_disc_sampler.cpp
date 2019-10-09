#include "poisson_disc_sampler.hpp"

using namespace Terra::Grid;

PoissonDiscSampler::PoissonDiscSampler()
{
}

PoissonDiscSampler::PoissonDiscSampler(std::vector<Terra::Vector2>& points, Terra::Grid::HashGrid& grid, int64_t sizeX, int64_t sizeY, double radius, int64_t samples)
{
    this->radius = radius;
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    this->samples = samples;
    this->points = &points;
    this->grid = &grid;
}

int64_t PoissonDiscSampler::Sample()
{
    double inner = this->radius * this->radius;
    double outer = (4 * this->radius * this->radius) - inner;
    int64_t count = 0;

    std::vector<int64_t> active;

    double bx0 = this->radius;
    double by0 = this->radius;
    double bx1 = this->sizeX - this->radius;
    double by1 = this->sizeY - this->radius;

    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<double> normal(0.0, 1.0); // Random double between 0.0 and 1.0

    { // create the initial point
        std::uniform_real_distribution<double> dis(0.0, (double)std::min(sizeX, sizeY)); // Random double within the grid

        Vector2 point(Terra::Vector2(dis(gen), dis(gen)));
        this->points->push_back(point);
        active.push_back(0);

        this->grid->Set(point, count);
    }

    while (!active.empty())
    {
        int64_t i = normal(gen) * (active.size() - 1); // Get a random index from the list of active points

        for (int64_t j = 0; j < this->samples; j++) // Loop till a valid point is found or the sample limit is reached
        {
            // Create new point around current index
            double theta = normal(gen) * 2.0 * pi; // Random radian on the circumference of the circle
            double r = std::sqrt((normal(gen) * outer) + inner); // Random radius of the circle between r^2 and 4r
            // Calculate the position of the point on the circle
            
            auto p = this->points->at(i);
            Terra::Vector2 point
            (
                p.x + (r * std::cos(theta)),
                p.y + (r * std::sin(theta))
            );

            if (bx0 <= point.x && point.x <= bx1 && by0 <= point.y && point.y <= by1) // Is within max extents
            {
                bool valid = true;
                for (int64_t index : this->grid->Neighbours(point))
                {
                    // If the point is to close to neighbour this point should be ignored
                    if (Terra::Vector2::DistanceSquared(point, this->points->at(index)) <= inner)
                    {
                        valid = false;
                        break;
                    }
                }

                if (valid)
                {
                    // The point is valid add it to points then add the index to the active list
                    this->points->push_back(point);
                    count++;
                    active.push_back(count);
                    this->grid->Set(point, count);

                    break;
                }
            }

            if (j == this->samples)
            {
                active.erase(active.begin() + i);
            }
        }
    }

    return count + 1;
}