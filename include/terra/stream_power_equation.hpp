#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "base_types.hpp"
#include "math/sqrt.hpp"
#include "types/dynarray.hpp"
#include "types/flow_graph.hpp"
#include "types/vec2.hpp"

namespace terra
{
    class stream_power_equation
    {
        const tfloat m = 0.5f;
        // const tfloat n = 1.0f;

        tfloat k, dt;
        const std::span<const terra::vec2>* points;
        const terra::flow_graph* flow_graph;
        const std::span<const tfloat>* areas;
        const std::span<const tfloat>* uplifts;
        std::span<tfloat>* heights;

    public:
        stream_power_equation(tfloat k,
                              tfloat dt,
                              const std::span<const terra::vec2>& points,
                              const terra::flow_graph& flow_graph,
                              const std::span<const tfloat>& areas,
                              const std::span<const tfloat>& uplifts,
                              std::span<tfloat>& heights) :
            k(k), dt(dt), points(&points), flow_graph(&flow_graph),
            areas(&areas), uplifts(&uplifts), heights(&heights)
        {
        }

        void update()
        {
            for (const auto node : this->flow_graph->sorted_nodes)
            {
                const size_t flow_node = this->flow_graph->flow[node];
                if (flow_node == terra::flow_graph::node_external)
                {
                    continue;
                }

                this->heights->data()[node] = this->solve(node, flow_node);
            }
        }

    private:
        inline tfloat solve(const size_t i, const size_t j)
        {
            const tfloat hi = this->heights->data()[i];
            // drainage area of i
            const tfloat Ai = this->flow_graph->drainage_areas[i];
            // uplift at i multiplied by the time step
            const tfloat ui = this->uplifts->data()[i];

            if (j == terra::flow_graph::node_lake)
            {
                return hi + ui;
            }

            const tfloat hj = this->heights->data()[j];

            const tfloat dist =
                glm::distance(this->points->data()[i], this->points->data()[j]);
            const tfloat drainage_dist =
                (this->k * terra::math::sqrt(Ai)) / dist;
            return (hi + ui + (this->dt * (drainage_dist * hj))) /
                   (1.0 + (drainage_dist * this->dt));
        }
    };
} // namespace terra
