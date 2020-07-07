#pragma once

#include <span>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "base_types.hpp"
#include "math/radian.hpp"
#include "types/undirected_graph.hpp"
#include "types/vec2.hpp"
#include "utils/reverse.hpp"

namespace terra
{
    class thermal_erosion
    {
    public:
        thermal_erosion();
        thermal_erosion(const std::span<terra::vec2>& points,
                        std::span<tfloat>& heights,
                        const terra::undirected_graph& graph,
                        tfloat talus_angle = 40.0);

        void update();
    private:
        const std::span<terra::vec2>* m_points;
        std::span<tfloat>* m_heights;
        const terra::undirected_graph* m_graph;
        tfloat m_talus_slope;
    };
}
