#pragma once

#include <span>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "base_types.hpp"
#include "types/dynarray.hpp"
#include "types/flow_graph.hpp"
#include "types/vec2.hpp"

namespace terra
{
    class slope
    {
    public:
        terra::dynarray<tfloat> slopes;

    private:
        const std::span<const terra::vec2> points;
        const std::span<const tfloat> heights;
        const std::span<const size_t> flow;

    public:
        slope() : slopes(0), points(), heights(), flow()
        {
        }
        slope(const std::span<const terra::vec2>& points,
              const std::span<const tfloat>& heights,
              const std::span<const size_t>& flow) :
            slopes(points.size()), points(points), heights(heights), flow(flow)
        {
        }

        void update()
        {
            for (size_t i = 0; i < heights.size(); ++i)
            {
                const size_t flow_idx = this->flow[i];
                if (flow_idx != terra::flow_graph::node_lake)
                {
                    const auto dh = this->heights[flow_idx] - this->heights[i];
                    const auto dist = glm::distance(this->points[flow_idx], this->points[i]);
                    this->slopes[i] = dh / dist;
                }
                else
                {
                    this->slopes[i] = 0.0;
                }
            }
        }
    };
}