#include "terra/poisson_disc_sampler.hpp"

#include <chrono>
#define _USE_MATH_DEFINES
#include <cmath>
#include <limits>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "terra/utils/fast_sqrt.hpp"

using namespace terra;

poisson_disc_sampler::poisson_disc_sampler() :
    engine(std::chrono::system_clock::now().time_since_epoch().count()),
    distribution(0.0, 1.0)
{
}

poisson_disc_sampler::poisson_disc_sampler(double width,
                                           double height,
                                           double min_distance,
                                           int64_t max_attempts,
                                           terra::vec2 start) :
    engine(std::chrono::system_clock::now().time_since_epoch().count()),
    distribution(0.0, 1.0), width(width), height(height),
    min_distance(min_distance), max_attempts(max_attempts), start(start),
    cell_size(min_distance / std::sqrt(2)), count(0)
{
    this->grid_width = std::ceil(this->width / cell_size);
    this->grid_height = std::ceil(this->height / cell_size);
    this->grid.resize(grid_width * grid_height, terra::vec2(this->infinity, this->infinity));
}

poisson_disc_sampler::~poisson_disc_sampler()
{
}

int64_t poisson_disc_sampler::sample()
{
    if (this->start.x == infinity)
    {
        do
        {
            this->start.x = this->random(this->width);
            this->start.y = this->random(this->height);
        } while (!in_area(this->start));
    }

    this->add(this->start);

    while (!(this->active.empty()))
    {
        auto point = this->active.top();
        this->active.pop();

        for (int64_t i = 0; i != this->max_attempts; ++i)
        {
            auto p = this->point_around(point);

            if (this->in_area(p) && !(this->point_too_close(p)))
            {
                this->add(p);
            }
        }
    }

    return count;
}

double poisson_disc_sampler::random(float range)
{
    return this->distribution(engine) * range;
}

terra::vec2 poisson_disc_sampler::point_around(terra::vec2 p)
{
    constexpr auto M_PI = 3.141592653589793238462643383279502884;

    auto radius = this->min_distance * std::sqrt(this->random(3) + 1);
    auto angle = this->random(2 * M_PI);

    p.x += std::cos(angle) * radius;
    p.y += std::sin(angle) * radius;

    return p;
}

bool poisson_disc_sampler::in_area(const terra::vec2& p)
{
    return p.x > 0 && p.x < this->width && p.y > 0 && p.y < this->height;
}

void poisson_disc_sampler::set(const terra::vec2& p)
{
    int x = p.x / cell_size;
    int y = p.y / cell_size;
    this->grid[y * this->grid_width + x] = p;
}

void poisson_disc_sampler::add(const terra::vec2& p)
{
    ++(this->count);
    this->active.push(p);
    this->set(p);
};

bool poisson_disc_sampler::point_too_close(const terra::vec2& p)
{
    int64_t x_index = std::floor(p.x / cell_size);
    int64_t y_index = std::floor(p.y / cell_size);

    if (this->grid[y_index * this->grid_width + x_index].x != infinity)
    {
        return true;
    }

    auto min_dist_squared = this->min_distance * this->min_distance;
    auto min_x = std::max(x_index - 2, 0ll);
    auto min_y = std::max(y_index - 2, 0ll);
    auto max_x = std::min(x_index + 2, grid_width - 1);
    auto max_y = std::min(y_index + 2, grid_height - 1);

    for (auto y = min_y; y <= max_y; ++y)
    {
        for (auto x = min_x; x <= max_x; ++x)
        {
            auto point = grid[y * grid_width + x];
            auto exists = point.x != infinity;

            if (exists && glm::distance2(p, point) < min_dist_squared)
            {
                return true;
            }
        }
    }

    return false;
};
