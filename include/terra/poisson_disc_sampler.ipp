#pragma once

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

template<terra::Vec2 PointConfig>
terra::poisson_disc_sampler<PointConfig>::poisson_disc_sampler() :
    m_width(0), m_height(0), m_min_distance(0.0),
    m_engine(std::chrono::system_clock::now().time_since_epoch().count()),
    m_distribution(0.0, 1.0), m_points(), m_active(), m_hash_grid()
{
}

template<terra::Vec2 PointConfig>
terra::poisson_disc_sampler<PointConfig>::~poisson_disc_sampler()
{
}

template<terra::Vec2 PointConfig>
std::vector<typename PointConfig::point_type>
terra::poisson_disc_sampler<PointConfig>::sample
(
    size_t width,
    size_t height,
    value_type min_distance,
    size_t max_attempts,
    grid_type& hash_grid,
    point_type start
)
{
    m_width        = width;
    m_height       = height;
    m_min_distance = min_distance;
    m_hash_grid    = hash_grid;
    m_hash_grid.resize(width, height, min_distance);

    // initialise random starting point if one is not passed to the function
    if (start.x == infinity)
    {
        do
        {
            start.x = m_random(m_width);
            start.y = m_random(m_height);
        } while (!in_area(start));
    }

    this->add(start);

    while (!(m_active.empty()))
    {
        auto point = m_points[m_active.top()];
        m_active.pop();

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
    m_points.shrink_to_fit();
    return m_points;
}

template<terra::Vec2 PointConfig>
typename PointConfig::value_type
terra::poisson_disc_sampler<PointConfig>::random(value_type range)
{
    return m_distribution(m_engine) * range;
}

template<terra::Vec2 PointConfig>
typename terra::poisson_disc_sampler<PointConfig>::point_type
terra::poisson_disc_sampler<PointConfig>::point_around(const point_type& p)
{
    auto radius = m_min_distance * std::sqrt(random(3) + 1);
    auto angle = m_random(2 * terra::math::PI);

    p.x += std::cos(angle) * radius;
    p.y += std::sin(angle) * radius;

    return p;
}

template<terra::Vec2 PointConfig>
bool terra::poisson_disc_sampler<PointConfig>::in_area(const point_type& p)
{
    return p.x > 0 && p.x < m_width && p.y > 0 && p.y < m_height;
}

template<terra::Vec2 PointConfig>
void terra::poisson_disc_sampler<PointConfig>::add(const point_type& p)
{
    size_t index = m_points.size();

    m_active.push(index);
    m_hash_grid->set(p, index);

    m_points.push_back(p);
}

template<terra::Vec2 PointConfig>
bool terra::poisson_disc_sampler<PointConfig>::point_too_close(const point_type& p)
{
    if (!m_hash_grid->is_cell_empty(p))
    {
        return true;
    }

    const tfloat min_dist_squared = m_min_distance * m_min_distance;
    for (const size_t n : m_hash_grid->get_neighbours(p))
    {
        if (glm::distance2(p, m_points[n]) < min_dist_squared)
        {
            return true;
        }
    }

    return false;
}
