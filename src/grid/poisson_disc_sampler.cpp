#include "poisson_disc_sampler.hpp"

#include "../utils/fast_sqrt.hpp"

using namespace Terra::Grid;

PoissonDiscSampler::PoissonDiscSampler() :
    radius(0.0),
    sizeX(0),
    sizeY(0),
    samples(0),
    points(),
    grid(),
    inner(0.0),
    outer(0.0),
    count(0),
    bounds(),
    rd(),
    gen(),
    normal()
{
}

PoissonDiscSampler::PoissonDiscSampler(std::vector<Terra::Vector2>& points, Terra::HashGrid& grid, int64_t sizeX, int64_t sizeY, double radius, int64_t samples) :
    radius(radius),
    sizeX(sizeX),
    sizeY(sizeY),
    samples(samples),
    points(&points),
    grid(&grid),
    count(0),
    rd(),
    gen(rd()),
    normal(0.0, 1.0)
{
    inner = this->radius * this->radius;
    outer = 3 * inner;

    bounds = Rect(this->radius, this->radius, this->sizeX - this->radius, this->sizeY - this->radius);
}

int64_t PoissonDiscSampler::Sample()
{
    { // create the initial point
        std::uniform_real_distribution<double> dis(0.0, static_cast<double>(std::min(sizeX, sizeY))); // Random double within the grid

        Vector2 point(Terra::Vector2(dis(gen), dis(gen)));
        this->points->push_back(point);
        active.push_back(0);

        this->grid->Set(point, count);
        count++;
    }

    while (!active.empty())
    {
        int64_t i = Utils::FastRound<int64_t>(normal(gen) * static_cast<double>(active.size() - 1)); // Get a random index from the list of active points

        for (int64_t j = 0; j < this->samples; j++) // Loop till a valid point is found or the sample limit is reached
        {
            // Create new point around current index
            Terra::Vector2 point = this->GenerateAround(this->points->at(i));
            if (this->IsValid(point))
            {
                // The point is valid add it to points then add the index to the active list
                this->points->push_back(point);
                active.push_back(count);
                this->grid->Set(point, count);
                count++;

                break;
            }

            if (j == this->samples)
            {
                this->active.erase(active.begin() + i);
            }
        }
    }

    return count;
}

inline const Terra::Vector2 PoissonDiscSampler::GenerateAround(Terra::Vector2& p)
{
    double theta = this->normal(this->gen) * 2.0 * this->pi; // Random radian on the circumference of the circle
    double r = Utils::FastSqrt((this->normal(gen) * this->outer) + this->inner); // Random radius of the circle between r^2 and 4r
    // Calculate the position of the point on the circle
    
    return Terra::Vector2
    (
        p.x + (r * std::cos(theta)),
        p.y + (r * std::sin(theta))
    );
}

constexpr bool PoissonDiscSampler::IsValid(Terra::Vector2& p)
{
    if (!this->bounds.WithinExtent(p)) // Is within max extents
    {
        return false;
    }

    for (int64_t index : this->grid->Neighbours(p))
    {
        // If the point is to close to neighbour this point should be ignored
        if (Terra::Vector2::DistanceSquared(p, this->points->at(index)) <= inner)
        {
            return false;
        }
    }

    return true;
}
