#include "terra/poisson_disc_sampler.hpp"

#include <chrono>
#define _USE_MATH_DEFINES
#include <cmath>
#include <limits>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "terra/math/floor.hpp"
#include "terra/math/pi.hpp"
#include "terra/math/sqrt.hpp"

using namespace terra;

poisson_disc_sampler::poisson_disc_sampler() :
    width(0), height(0), min_distance(0.0),
    engine(std::chrono::system_clock::now().time_since_epoch().count()),
    distribution(0.0, 1.0),
    points(), active(), hash_grid(nullptr)
{
}

poisson_disc_sampler::~poisson_disc_sampler()
{
}

std::vector<terra::vec2> poisson_disc_sampler::sample
(
    size_t width,
    size_t height,
    tfloat min_distance,
    size_t max_attempts,
    terra::hash_grid** hash_grid,
    terra::vec2 start
)
{
    this->width = width;
    this->height = height;
    this->min_distance = min_distance;

    this->hash_grid = new terra::hash_grid(width, height, min_distance);
    if (hash_grid != nullptr)
    {
        *hash_grid = this->hash_grid;
    }

    // with a area of 50,000 x 50,000 and r = 100.0
    // there should be ~160,000 points
    const size_t point_estimate = 0.8 *
                                  ((width * height) /
                                  (min_distance * min_distance));
    this->points.reserve(point_estimate);

    // initialise random starting point if one is not passed to the function
    if (start.x == infinity)
    {
        do
        {
            start.x = this->random(this->width);
            start.y = this->random(this->height);
        } while (!in_area(start));
    }

    this->add(start);

    while (!(this->active.empty()))
    {
        auto point = this->points[this->active.top()];
        this->active.pop();

        for (size_t i = 0; i < max_attempts; ++i)
        {
            auto p = this->point_around(point);

            if (this->in_area(p) && !(this->point_too_close(p)))
            {
                this->add(p);
            }
        }
    }

    // shrink the vector so that the excess memory is freed
    this->points.shrink_to_fit();
    return this->points;
}

tfloat poisson_disc_sampler::random(tfloat range)
{
    return this->distribution(engine) * range;
}

terra::vec2 poisson_disc_sampler::point_around(terra::vec2 p)
{
    auto radius = this->min_distance * std::sqrt(this->random(3) + 1);
    auto angle = this->random(2 * terra::math::PI);

    p.x += std::cos(angle) * radius;
    p.y += std::sin(angle) * radius;

    return p;
}

bool poisson_disc_sampler::in_area(const terra::vec2& p)
{
    return p.x > 0 && p.x < this->width && p.y > 0 && p.y < this->height;
}

void poisson_disc_sampler::add(const terra::vec2& p)
{
    size_t index = this->points.size();

    this->active.push(index);
    this->hash_grid->set(p, index);

    this->points.push_back(p);
}

bool poisson_disc_sampler::point_too_close(const terra::vec2& p)
{
    if (!this->hash_grid->is_cell_empty(p))
    {
        return true;
    }

    const tfloat min_dist_squared = this->min_distance * this->min_distance;
    for (const size_t n : this->hash_grid->get_neighbours(p))
    {
        if (glm::distance2(p, this->points[n]) < min_dist_squared)
        {
            return true;
        }
    }

    return false;
}
