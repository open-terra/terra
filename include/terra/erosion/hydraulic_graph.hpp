#pragma once

#include <span>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "hydraulic_config.hpp"
#include "../base_types.hpp"
#include "../types/dynarray.hpp"
#include "../types/flow_graph.hpp"
#include "../types/hash_grid.hpp"
#include "../types/triangle.hpp"
#include "../types/rect.hpp"
#include "../types/vec2.hpp"

namespace terra::erosion
{
    class hydraulic_graph
    {
    public:
        hydraulic_graph(const std::span<terra::vec2>& points,
                        const terra::hash_grid& hash_grid,
                        const std::span<terra::triangle>& triangles,
                        std::span<tfloat>& heights,
                        size_t seed);

        void erode(size_t droplets, const terra::rect<tfloat>& bounds);

        hydraulic_config m_config;

    private:
        terra::dynarray<std::list<size_t>> tri_map;
        terra::dynarray<terra::dynarray<std::pair<size_t, tfloat>>> erosion_map;

        const std::span<terra::vec2>* points;
        const terra::hash_grid* hash_grid;
        std::span<tfloat>* heights;
        const std::span<terra::triangle>* triangles;

        size_t droplet_node(const terra::vec2& p);
        const triangle& droplet_tri(const terra::vec2& p, size_t node);
        tfloat droplet_height(const terra::vec3& p0, const terra::vec3& p1, const terra::vec3& p2, const terra::vec2& p); 
        terra::vec2 droplet_gradient(const terra::vec3& p0, const terra::vec3& p1, const terra::vec3& p2, const terra::vec3& p);
        terra::vec2 droplet_direction(const terra::vec2& grad, const terra::vec2& dir);
        tfloat droplet_slope(const terra::vec3& p0, const terra::vec3& p1, const terra::vec3& p2, const terra::vec2& p);

        inline auto get_tri_points(const terra::triangle& tri)
        {
            const auto& p0 = this->points->data()[tri.v0];
            const auto& p1 = this->points->data()[tri.v1];
            const auto& p2 = this->points->data()[tri.v2];
            const auto h0 = this->heights->data()[tri.v0];
            const auto h1 = this->heights->data()[tri.v1];
            const auto h2 = this->heights->data()[tri.v2];

            return std::make_tuple(terra::make_vec3(p0, h0),
                                   terra::make_vec3(p1, h2),
                                   terra::make_vec3(p1, h2));
        }
    };
}
