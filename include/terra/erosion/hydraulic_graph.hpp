#pragma once

#include <span>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "hydraulic_config.hpp"
#include "../base_types.hpp"
#include "../concepts.hpp"
#include "../concepts_helpers.hpp"
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
        template<class ArrayVec, class ArrayTriangle, class ArrayReal>
        requires terra::Container<ArrayVec, terra::vec2> &&
                 terra::Container<ArrayTriangle, terra::triangle>&&
                 terra::Container<ArrayReal, tfloat>
        inline hydraulic_graph(const ArrayVec& points,
                               const terra::hash_grid& hash_grid,
                               const ArrayTriangle& triangles,
                               ArrayReal& heights,
                               size_t seed);

        void erode(size_t droplets, const terra::rect<tfloat>& bounds);

        hydraulic_config m_config;

    private:
        terra::dynarray<std::list<size_t>> tri_map;
        terra::dynarray<terra::dynarray<std::pair<size_t, tfloat>>> erosion_map;

        const std::span<const terra::vec2> points;
        const terra::hash_grid* hash_grid;
        const std::span<const terra::triangle> triangles;
        std::span<tfloat> heights;

        void init();
        size_t droplet_node(const terra::vec2& p);
        const triangle& droplet_tri(const terra::vec2& p, size_t node);
        tfloat droplet_height(const terra::vec3& p0, const terra::vec3& p1, const terra::vec3& p2, const terra::vec2& p); 
        terra::vec2 droplet_gradient(const terra::vec3& p0, const terra::vec3& p1, const terra::vec3& p2, const terra::vec3& p);
        terra::vec2 droplet_direction(const terra::vec2& grad, const terra::vec2& dir);
        tfloat droplet_slope(const terra::vec3& p0, const terra::vec3& p1, const terra::vec3& p2, const terra::vec2& p);

        inline auto get_tri_points(const terra::triangle& tri)
        {
            const auto& p0 = this->points[tri.v0];
            const auto& p1 = this->points[tri.v1];
            const auto& p2 = this->points[tri.v2];
            const auto h0 = this->heights[tri.v0];
            const auto h1 = this->heights[tri.v1];
            const auto h2 = this->heights[tri.v2];

            return std::make_tuple(terra::make_vec3(p0, h0),
                                   terra::make_vec3(p1, h2),
                                   terra::make_vec3(p2, h2));
        }
    };
}

template<class ArrayVec, class ArrayTriangle, class ArrayReal>
requires terra::Container<ArrayVec, terra::vec2>&&
         terra::Container<ArrayTriangle, terra::triangle>&&
         terra::Container<ArrayReal, tfloat>
terra::erosion::hydraulic_graph::hydraulic_graph(
    const ArrayVec& points,
    const terra::hash_grid& hash_grid,
    const ArrayTriangle& triangles,
    ArrayReal& heights,
    size_t seed) :
    m_config(), tri_map(points.size()), erosion_map(points.size()),
    points(terra::to_span<const terra::vec2>(points)), hash_grid(&hash_grid),
    triangles(terra::to_span<const terra::triangle>(triangles)),
    heights(terra::to_span<tfloat>(heights))
{
    this->init();
}

