#include "terra/erosion/hydraulic_grid.hpp"

#include <algorithm>
#include <random>

#include "terra/math/abs.hpp"
#include "terra/math/ceil.hpp"
#include "terra/math/floor.hpp"
#include "terra/math/lerp.hpp"
#include "terra/math/sqrt.hpp"

void terra::erosion::hydraulic_grid::init()
{
    const size_t iradius = terra::math::ceil<size_t>(this->m_config.erosion_radius);
    const auto radius = this->m_config.erosion_radius;
    const auto r2 = radius * radius;

    terra::dynarray<std::pair<size_t, tfloat>> weights(iradius * iradius * 4);

    for (size_t y = 0; y < height; ++y)
    {
        const size_t offset_y = y * height;
        for (size_t x = 0; x < width; ++x)
        {
            const size_t offset = offset_y + x;

            size_t count = 0;
            tfloat weight_sum = 0.0;

            const auto min_x = std::max<size_t>(x - radius, 0ull);
            const auto min_y = std::max<size_t>(x - radius, 0ull);
            const auto max_x = std::min<size_t>(x + radius, width);
            const auto max_y = std::min<size_t>(x + radius, height);
            for (size_t i = min_y; i < max_y; ++i)
            {
                const size_t offset_i = i * height;
                for (size_t j = min_x; j < max_x; ++j)
                {
                    const auto offset_ij = offset_i + j;
                    const auto dx = j - x;
                    const auto dy = i - y;
                    const auto dist2 = static_cast<tfloat>((dx * dx) + (dy * dy));
                    if (dist2 > r2)
                    {
                        continue;
                    }

                    const auto dist = terra::math::sqrt(dist2);
                    weights[count] = std::make_pair(offset_ij, dist);
                    weight_sum += dist;
                    ++count;
                }
            }

            this->m_weights[offset] = terra::dynarray<std::pair<size_t, tfloat>>(count);
            for (size_t i = 0; i < count; ++i)
            {
                const auto& p = weights[i];
                this->m_weights[offset][i] = std::make_pair(p.first,
                                                            p.second / weight_sum);
            }
        }
    }
}

void terra::erosion::hydraulic_grid::erode(size_t droplets, 
                                           const terra::rect<tfloat>& bounds)
{
    std::default_random_engine engine(this->m_config.seed);
    std::uniform_real_distribution<tfloat> dist_x(bounds.x0, bounds.x1);
    std::uniform_real_distribution<tfloat> dist_y(bounds.y0, bounds.y1);

    for (size_t i = 0; i < droplets; ++i)
    {
        // rain drop within the bounds
        auto p = terra::vec2(dist_x(engine), dist_y(engine));
        auto dir = terra::vec2(0, 0);
        tfloat speed = this->m_config.initial_speed;
        tfloat water = this->m_config.initial_water_volume;
        tfloat sediment = 0.0;

        // get the initail values
        // find the nearest node
        auto [dx, dy] = this->droplet_node(p);

        auto dw = this->droplet_node_weights(p, dx, dy);

        auto [n00, n01, n10, n11] = this->droplet_nodes(p, dx, dy);

        // find the height of the water drop
        auto h = this->droplet_height(dw, n00, n01, n10, n11);

        for (size_t j = 0; j < this->m_config.max_droplet_lifetime; ++j)
        {
            // get the gradient for the droplet
            auto grad = this->droplet_gradient(dw, n00, n01, n10, n11);

            const tfloat inertia = this->m_config.inertia;
            dir = glm::normalize((dir * inertia) - (grad * (1 - inertia)));

            p += dir;

            // stop simulating droplet if it's not moving or has flowed over
            // edge of map
            if ((dir.x == 0.0 && dir.y == 0.0) ||
                p.x < 0.0 || p.x >= this->width - 1 ||
                p.y < 0.0 || p.y >= this->height - 1)
            {
                break;
            }

            std::tie(dx, dy) = this->droplet_node(p);
            dw = this->droplet_node_weights(p, dx, dy);
            std::tie(n00, n01, n10, n11) = this->droplet_nodes(p, dx, dy);

            // get the height for the droplet
            auto nh = this->droplet_height(dw, n00, n01, n10, n11);
            auto dh = nh - h;

            auto sediment_capacity = std::max(
                -dh * speed * water * this->m_config.sediment_capacity_factor,
                this->m_config.min_sediment_capacity);

            if (sediment > sediment_capacity || dh > 0.0)
            {
                // If moving uphill (deltaHeight > 0) try fill up to the current
                // height, otherwise deposit a fraction of the excess sediment
                auto amount_to_deposit = dh > 0.0 ?
                    std::min(dh, sediment) :
                    (sediment - sediment_capacity) * this->m_config.deposit_speed;
                sediment -= amount_to_deposit;

                // Add the sediment to the four nodes of the current cell using
                // bilinear interpolation
                // Deposition is not distributed over a radius (like erosion) so
                // that it can fill small pits
                this->m_heights[n00] += amount_to_deposit * (1 - dw.x) * (1 - dw.y);
                this->m_heights[n01] += amount_to_deposit * dw.x * (1 - dw.y);
                this->m_heights[n10] += amount_to_deposit * (1 - dw.x) * dw.y;
                this->m_heights[n11] += amount_to_deposit * dw.x * dw.y;
            }
            else
            {
                auto amount_to_erode = std::min(
                    (sediment_capacity - sediment) * this->m_config.erode_speed,
                    -dh);
                
                for (const auto& n : this->m_weights[n00])
                {
                    auto weighed_erode_amount = amount_to_erode * n.second;
                    auto ds = this->m_heights[n.first] < weighed_erode_amount ?
                        this->m_heights[n.first] :
                        weighed_erode_amount;

                    this->m_heights[n.first] -= ds;
                    sediment += ds;
                }
            }

            // update droplet's speed and water content
            speed = terra::math::sqrt((speed * speed) + (dh * this->m_config.gravity));
            water *= (1 - this->m_config.evaporate_speed);

            // copy new values
            h = nh;
        }
    }
}

std::tuple<size_t, size_t> terra::erosion::hydraulic_grid::droplet_node
(
    const terra::vec2& p
)
{
    return std::make_tuple(terra::math::floor<size_t>(p.x),
                           terra::math::floor<size_t>(p.y));
}

std::tuple<size_t, size_t, size_t, size_t>
terra::erosion::hydraulic_grid::droplet_nodes
(
    const terra::vec2& p,
    size_t x,
    size_t y
)
{
    // n10-n11
    //  | * |
    // n00-n01
    const size_t n00 = (y * this->width) + x;
    const size_t n01 = n00 + 1;
    const size_t n10 = n00 + this->width;
    const size_t n11 = n10 + 1;

    return std::make_tuple(n00, n01, n10, n11);
}

terra::vec2 terra::erosion::hydraulic_grid::droplet_node_weights
(
    const terra::vec2& p,
    size_t x,
    size_t y
)
{
    return terra::vec2(terra::math::abs(p.x - x),
                       terra::math::abs(p.y - y));
}

tfloat terra::erosion::hydraulic_grid::droplet_height
(
    const terra::vec2& weights,
    size_t n00,
    size_t n01,
    size_t n10,
    size_t n11
)
{
    const auto h00 = this->m_heights[n00];
    const auto h01 = this->m_heights[n01];
    const auto h10 = this->m_heights[n10];
    const auto h11 = this->m_heights[n11];

    const auto h00_01 = terra::math::lerp(h00, h01, weights.x);
    const auto h10_11 = terra::math::lerp(h10, h11, weights.x);

    return terra::math::lerp(h00_01, h10_11, weights.y);
}

terra::vec2 terra::erosion::hydraulic_grid::droplet_gradient
(
    const terra::vec2& weights,
    size_t n00,
    size_t n01,
    size_t n10,
    size_t n11
)
{
    const auto h00 = this->m_heights[n00];
    const auto h01 = this->m_heights[n01];
    const auto h10 = this->m_heights[n10];
    const auto h11 = this->m_heights[n11];

    const tfloat gx = ((h10 - h11) * (1.f - weights.x)) +
                      ((h00 - h01) * weights.x);

    const tfloat gy = ((h00 - h10) * (1.f - weights.y)) +
                      ((h01 - h11) * weights.y);

    return terra::vec2(gx, gy);
}
