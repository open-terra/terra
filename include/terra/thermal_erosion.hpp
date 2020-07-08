#pragma once

#include <span>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "base_types.hpp"
#include "concepts.hpp"
#include "concepts_helpers.hpp"
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
        template<class ArrayVec, class ArrayReal>
        requires terra::Container<ArrayVec, terra::vec2>&&
                 terra::Container<ArrayReal, tfloat>
        inline thermal_erosion(const ArrayVec& points,
                               ArrayReal& heights,
                               const terra::undirected_graph& graph,
                               tfloat talus_angle = 40.0);

        void update();
    private:
        const std::span<const terra::vec2> m_points;
        std::span<tfloat> m_heights;
        const terra::undirected_graph* m_graph;
        tfloat m_talus_slope;
    };
}

template<class ArrayVec, class ArrayReal>
requires terra::Container<ArrayVec, terra::vec2>&&
         terra::Container<ArrayReal, tfloat>
terra::thermal_erosion::thermal_erosion(const ArrayVec& points,
                                        ArrayReal& heights,
                                        const terra::undirected_graph& graph,
                                        tfloat talus_angle) :
    m_points(terra::to_span<const terra::vec2>(points)),
    m_heights(terra::to_span<tfloat>(heights)), m_graph(&graph),
    m_talus_slope(std::tan(terra::math::to_radian(talus_angle)))
{
}
