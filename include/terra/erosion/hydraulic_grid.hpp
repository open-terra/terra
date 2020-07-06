#pragma once

#include <span>
#include <utility>

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
    class hydraulic_grid
    {
    public:
        hydraulic_grid(size_t width,
                       size_t height,
                       std::span<tfloat>& heights,
                       hydraulic_config config);

        void erode(size_t droplets, const terra::rect<tfloat>& bounds);

        hydraulic_config m_config;

    private:
        size_t width, height;
        terra::dynarray<terra::dynarray<std::pair<size_t, tfloat>>> m_weights;
        std::span<tfloat>* m_heights;

        std::tuple<size_t, size_t> droplet_node(const terra::vec2& p);
        std::tuple<size_t, size_t, size_t, size_t>droplet_nodes(const terra::vec2& p, size_t x, size_t y);
        terra::vec2 droplet_node_weights(const terra::vec2& p,
                                         size_t x,
                                         size_t y);
        tfloat droplet_height(const terra::vec2& weights,
                              size_t n00,
                              size_t n01,
                              size_t n10,
                              size_t n11);
        terra::vec2 droplet_gradient(const terra::vec2& weights,
                                     size_t n00,
                                     size_t n01,
                                     size_t n10,
                                     size_t n11);
        terra::vec2 droplet_direction(const terra::vec2& grad, const terra::vec2& dir);
        tfloat droplet_slope(const terra::vec3& p0, const terra::vec3& p1, const terra::vec3& p2, const terra::vec2& p);
    };
}
