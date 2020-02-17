#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "base_types.hpp"
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
        const std::vector<terra::vec2>* points;
        const terra::flow_graph* flow_graph;
        const terra::dynarray<tfloat>* areas;
        const terra::dynarray<tfloat>* uplifts;
        terra::dynarray<tfloat>* heights;

    public:
        stream_power_equation(tfloat k,
                              tfloat dt,
                              const std::vector<terra::vec2>& points,
                              const terra::flow_graph& flow_graph,
                              const terra::dynarray<tfloat>& areas,
                              const terra::dynarray<tfloat>& uplifts,
                              terra::dynarray<tfloat>& heights) :
            k(k), dt(dt), points(&points), flow_graph(&flow_graph),
            areas(&areas), uplifts(&uplifts), heights(&heights)
        {
        }

        void update()
        {
            for (size_t i = heights->size() - 1; i >= 0; --i)
            {
                const size_t node = this->flow_graph->sorted_nodes[i];
                const size_t flow_node = this->flow_graph->flow[node];
                this->heights->at(node) = this->solve(node, flow_node);
            }
        }

    private:
        inline tfloat solve(const size_t i,
                            const size_t j)
        {
            const tfloat hi = this->heights->at(i);
            const tfloat ai = this->areas->at(i);
            const tfloat ui = this->uplifts->at(i);

            const tfloat hj = j == terra::flow_graph::node_lake ? 0.0 : this->heights->at(j);

            const tfloat dist = glm::distance(this->points->at(i), this->points->at(j));
            const tfloat drainage_dist = (this->k * std::pow(ai, this->m)) / dist;
            return
                (hi + (this->dt * (ui + (drainage_dist * hj)))) /
                (1 + (drainage_dist * this->dt));
        }
    };
}