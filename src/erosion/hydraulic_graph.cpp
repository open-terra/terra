#include "terra/erosion/hydraulic_graph.hpp"

#include <random>

#include "terra/math/ceil.hpp"
#include "terra/math/sqrt.hpp"

terra::erosion::hydraulic_graph::hydraulic_graph(
    const std::span<terra::vec2>& points,
    const terra::hash_grid& hash_grid,
    const std::span<terra::triangle>& triangles,
    std::span<tfloat>& heights,
    size_t seed) :
    m_config(),
    tri_map(points.size()),
    erosion_map(points.size()),
    points(&points),
    hash_grid(&hash_grid),
    heights(&heights),
    triangles(&triangles)
{
    for (size_t i = 0; i < this->triangles->size(); ++i)
    {
        const auto& tri = this->triangles->data()[i];
        for (auto j : tri.get_vert_idx())
        {
            this->tri_map[j].push_back(i);
        }
    }

    size_t neighbours_radius =
        terra::math::ceil<size_t>(this->m_config.erosion_radius /
                                  this->hash_grid->sqrt1_2);

    for (size_t i = 0; i < this->points->size(); ++i)
    {
        const auto& p = this->points->data()[i];
        auto neighbours = this->hash_grid->get_neighbours(p, neighbours_radius);
        this->erosion_map[i] = terra::dynarray<std::pair<size_t, tfloat>>(neighbours.size());

        tfloat total_weight = 0.0;
        for (size_t j = 0; j < neighbours.size(); ++j)
        {
            const auto& pn = this->points->data()[neighbours[j]];

            auto weight = glm::distance(p, pn) / this->m_config.erosion_radius;
            this->erosion_map[i][j] = std::make_pair(neighbours[j], weight);

            total_weight += weight;
        }

        for (size_t j = 0; j < neighbours.size(); ++j)
        {
            this->erosion_map[i][j].second /= total_weight;
        }
    }
}

void terra::erosion::hydraulic_graph::erode(size_t droplets,
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
        auto dn = this->droplet_node(p);

        // find the triangle that the point lies within
        auto tri = this->droplet_tri(p, dn);
        auto [p0, p1, p2] = this->get_tri_points(tri);

        // find the height of the water drop
        auto h = this->droplet_height(p0, p1, p2, p);

        for (size_t j = 0; j < this->m_config.max_droplet_lifetime; ++j)
        {
            // get the gradient for the droplet to the lowest edge of the
            // current tri
            auto grad = this->droplet_gradient(p0, p1, p2, terra::make_vec3(p, h));

            // update the direction
            dir = this->droplet_direction(grad, dir);

            // move the point down hill
            p += dir;

            // stop simulating droplet if it's not moving or has flowed over 
            // edge of map
            if (!bounds.within_extent(p))
            {
                break;
            }

            // find the nearest node
            auto nn = this->droplet_node(p);

            // find the triangle that the point lies within
            auto ntri = this->droplet_tri(p, nn);
            auto [np0, np1, np2] = this->get_tri_points(ntri);

            // find the height of the water drop
            auto nh = this->droplet_height(np0, np1, np2, p);

            auto dh = nh - h;

            auto sediment_capacity = std::max(
                -dh * speed * water * this->m_config.sediment_capacity_factor,
                this->m_config.min_sediment_capacity);

            if (sediment > sediment_capacity || dh > 0.0)
            {
                auto amount_to_deposit = dh > 0.0 ? 
                    std::min(dh, sediment) :
                    (sediment - sediment_capacity) * this->m_config.deposit_speed;

                auto np = terra::make_vec3(p, nh);
                auto d0 = glm::distance(np0, np);
                auto d1 = glm::distance(np1, np);
                auto d2 = glm::distance(np2, np);
                auto total = d0 + d1 + d2;

                this->heights->data()[ntri.v0] += amount_to_deposit * (d0 / total);
                this->heights->data()[ntri.v1] += amount_to_deposit * (d1 / total);
                this->heights->data()[ntri.v2] += amount_to_deposit * (d2 / total);
            }
            else
            {
                auto amount_to_erode = std::min(
                    (sediment_capacity - sediment) * this->m_config.erode_speed,
                    -dh);

                for (const auto& n : this->erosion_map[nn])
                {
                    auto weighed_erode_amount = amount_to_erode * n.second;
                    auto ds = this->heights->data()[n.first] < weighed_erode_amount ?
                        this->heights->data()[n.first] :
                        weighed_erode_amount;

                    this->heights->data()[n.first] -= ds;
                    sediment += ds;
                }
            }

            // update droplet's speed and water content
            speed = terra::math::sqrt((speed * speed) + (dh * this->m_config.gravity));
            water *= (1 - this->m_config.evaporate_speed);

            // update old values
            dn = nn;
            p0 = np0;
            p1 = np1;
            p2 = np2;
            h = nh;
        }
    }
}

size_t terra::erosion::hydraulic_graph::droplet_node(const terra::vec2& p)
{
    size_t node;
    // check if the cell that the rain drop landed in has a point, if so that
    // shall be the closest point
    if (this->hash_grid->is_cell_empty(p))
    {
        constexpr auto tfloat_max = std::numeric_limits<tfloat>::max();
        auto min = std::make_pair(terra::hash_grid::cell_empty, tfloat_max);
        for (auto n : this->hash_grid->get_neighbours(p, 2))
        {
            // get the squared distance
            tfloat nd = glm::distance2(p, this->points->data()[n]);
            if (nd < min.second)
            {
                min = std::make_pair(n, nd);
            }
        }

        node = min.first;
    }
    else
    {
        node = this->hash_grid->at(p);
    }

    return node;
}

const terra::triangle& terra::erosion::hydraulic_graph::droplet_tri
(
    const terra::vec2& p,
    size_t node
)
{
    for (auto t : this->tri_map[node])
    {
        const auto& tri = this->triangles->data()[t];

        const auto& p0 = this->points->data()[tri.v0];
        const auto& p1 = this->points->data()[tri.v1];
        const auto& p2 = this->points->data()[tri.v2];
        if (terra::triangle::point_within(p0, p1, p2, p))
        {
            return tri;
        }
    }

    // should throw as there should always be a triangle present
}

tfloat terra::erosion::hydraulic_graph::droplet_height(const terra::vec3& p0,
                                                 const terra::vec3& p1,
                                                 const terra::vec3& p2,
                                                 const terra::vec2& p)
{
    return terra::triangle::spatial_interp(p0, p1, p2, p);
}

terra::vec2 terra::erosion::hydraulic_graph::droplet_gradient
(
    const terra::vec3& p0,
    const terra::vec3& p1,
    const terra::vec3& p2,
    const terra::vec3& p
)
{
    // *__ p1
    // |  ‾‾* p2
    // | # / p
    // |  /
    // | /
    // |/
    // * p0

    auto d0 = p - p0;
    auto d1 = p - p1;
    auto d2 = p - p2;

    auto gradient = terra::vec2();
    if (d0.z >= d1.z && d0.z >= d2.z)
    {
        gradient = terra::vec2(d0.x / d0.z, d0.y / d0.z);
    }
    else if (d1.z >= d2.z)
    {
        gradient = terra::vec2(d1.x / d1.z, d1.y / d1.z);
    }
    else
    {
        gradient = terra::vec2(d2.x / d2.z, d2.y / d2.z);
    }

    return glm::normalize(gradient);
}

terra::vec2 terra::erosion::hydraulic_graph::droplet_direction
(
    const terra::vec2& grad,
    const terra::vec2& dir
)
{
    return terra::vec2(
        (dir.x * this->m_config.inertia) - (grad.x * (1 - this->m_config.inertia)),
        (dir.y * this->m_config.inertia) - (grad.y * (1 - this->m_config.inertia)));
}

tfloat terra::erosion::hydraulic_graph::droplet_slope(const terra::vec3& p0,
                                                      const terra::vec3& p1,
                                                      const terra::vec3& p2,
                                                      const terra::vec2& p)
{
    auto sn = terra::vec3(0.0, 0.0, 1.0);
    auto tn = terra::triangle::normal(p0, p1, p2);

    return glm::tan(glm::acos(glm::dot(sn, tn)));
}
