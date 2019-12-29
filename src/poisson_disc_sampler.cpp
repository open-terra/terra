#include "terra/poisson_disc_sampler.hpp"

#include <chrono>
#define _USE_MATH_DEFINES
#include <cmath>
#include <limits>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "terra/math/floor.hpp"
#include "terra/math/sqrt.hpp"

using namespace terra;

poisson_disc_sampler::poisson_disc_sampler() :
    engine(std::chrono::system_clock::now().time_since_epoch().count()),
    distribution(0.0, 1.0), width(1.0), height(1.0),
    min_distance(0.0),
    cell_size(0.0)
{
    this->grid_width = 0;
    this->grid_height = 0;
}

poisson_disc_sampler::~poisson_disc_sampler()
{
}

std::vector<terra::vec2> poisson_disc_sampler::sample(double width,
                                                      double height,
                                                      double min_distance,
                                                      size_t max_attempts,
                                                      terra::vec2 start)
{
    this->width = width;
    this->height = height;
    this->min_distance = min_distance;

    this->cell_size = min_distance / terra::math::sqrt(2);
    this->grid_width = std::ceil(this->width / cell_size);
    this->grid_height = std::ceil(this->height / cell_size);

    size_t grid_size = this->grid_width * this->grid_height;
    this->points.reserve(grid_size);
    this->grid.resize(grid_size, grid_empty);

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

        for (int64_t i = 0; i != max_attempts; ++i)
        {
            auto p = this->point_around(point);

            if (this->in_area(p) && !(this->point_too_close(p)))
            {
                this->add(p);
            }
        }
    }

    // shrink then move the vector to clear the data in the class and free
    // unused memory
    this->points.shrink_to_fit();
    return std::move(this->points);
}

double poisson_disc_sampler::random(float range)
{
    return this->distribution(engine) * range;
}

terra::vec2 poisson_disc_sampler::point_around(terra::vec2 p)
{
    constexpr double PI = 3.141592653589793238462643383279502884;

    auto radius = this->min_distance * std::sqrt(this->random(3) + 1);
    auto angle = this->random(2 * PI);

    p.x += std::cos(angle) * radius;
    p.y += std::sin(angle) * radius;

    return p;
}

bool poisson_disc_sampler::in_area(const terra::vec2& p)
{
    return p.x > 0 && p.x < this->width && p.y > 0 && p.y < this->height;
}

void poisson_disc_sampler::set(const terra::vec2& p, const size_t index)
{
    size_t x = math::floor<size_t>(p.x / this->cell_size);
    size_t y = math::floor<size_t>(p.y / this->cell_size);

    this->grid[y * this->grid_width + x] = index;
}

void poisson_disc_sampler::add(const terra::vec2& p)
{
    size_t index = this->points.size();

    this->active.push(index);
    this->set(p, index);

    this->points.push_back(p);
};

bool poisson_disc_sampler::point_too_close(const terra::vec2& p)
{
    size_t x_index = math::floor<size_t>(p.x / this->cell_size);
    size_t y_index = math::floor<size_t>(p.y / this->cell_size);

    if (this->grid[y_index * this->grid_width + x_index] != grid_empty)
    {
        return true;
    }

    auto min_dist_squared = this->min_distance * this->min_distance;
    auto min_x = std::max<size_t>(x_index - 2, 0ull);
    auto min_y = std::max<size_t>(y_index - 2, 0ull);
    auto max_x = std::min<size_t>(x_index + 2, this->grid_width - 1);
    auto max_y = std::min<size_t>(y_index + 2, this->grid_height - 1);

    for (auto y = min_y; y <= max_y; ++y)
    {
        for (auto x = min_x; x <= max_x; ++x)
        {
            size_t i = this->grid[y * grid_width + x];

            auto exists = i != grid_empty;
            if (exists && glm::distance2(p, this->points[i]) < min_dist_squared)
            {
                return true;
            }
        }
    }

    return false;
};
