#pragma once

#include <cstdint>
#include <random>
#include <stack>
#include <vector>

#include "types/hash_grid.hpp"

namespace terra
{
    template<Vec2 PointConfig>
    class poisson_disc_sampler
    {
    public:
        using config     = PointConfig;
        using point_type = typename PointConfig::point_type;
        using value_type = typename PointConfig::value_type;
        using grid_type  = terra::hash_grid<PointConfig>;

    public:
        constexpr static value_type infinity =
            std::numeric_limits<value_type>::infinity();

        poisson_disc_sampler();
        ~poisson_disc_sampler();

        std::vector<point_type> sample
        (
            size_t width,
            size_t height,
            value_type min_distance,
            size_t max_attempts = 30,
            grid_type& hash_grid = grid_type(width, height, min_distance),
            point_type start = { infinity, infinity }
        );

    private:
        value_type random(value_type range);
        point_type point_around(const point_type& p);
        bool in_area(const point_type& p);
        void add(const point_type& p);
        bool point_too_close(const point_type& p);

        value_type m_width;
        value_type m_height;

        value_type m_min_distance; // the minimum distance between two points

        std::default_random_engine                 m_engine;
        std::uniform_real_distribution<value_type> m_distribution;
        std::vector<point_type>                    m_points;
        std::stack<size_t>                         m_active;
        grid_type&                                 m_hash_grid;
    };
} // namespace terra

#include "poisson_disc_sampler.ipp"
