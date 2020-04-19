#pragma once

#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "base_types.hpp"
#include "math/radian.hpp"
#include "types/dynarray.hpp"
#include "types/undirected_graph.hpp"
#include "types/vec2.hpp"
#include "utils/enumerate.hpp"
#include "utils/reverse.hpp"

namespace terra
{
    class thermal_erosion
    {
    public:
        thermal_erosion();
        thermal_erosion(const std::vector<terra::vec2>& points,
                        terra::dynarray<tfloat>& heights,
                        const terra::undirected_graph& graph,
                        tfloat talus_angle = 40.0);

        void update();
    private:
        const std::vector<terra::vec2>* m_points;
        terra::dynarray<tfloat>* m_heights;
        const terra::undirected_graph* m_graph;
        tfloat m_talus_slope;
    };
}

terra::thermal_erosion::thermal_erosion() :
    m_points(nullptr),
    m_heights(nullptr),
    m_graph(nullptr),
    m_talus_slope(0.0)
{
}

terra::thermal_erosion::thermal_erosion(const std::vector<terra::vec2>& points,
                                        terra::dynarray<tfloat>& heights,
                                        const terra::undirected_graph& graph,
                                        tfloat talus_angle) :
    m_points(&points),
    m_heights(&heights),
    m_graph(&graph),
    m_talus_slope(std::tan(terra::math::to_radian(talus_angle)))
{
}

void terra::thermal_erosion::update()
{
    bool changed = true;
    while (changed)
    {
        changed = false;

        // iterate through the graph from highest -> lowest nodes, this should
        // allow for the
        for (size_t i = 0; i < this->m_points->size(); ++i)
        {
            const auto ih = this->m_heights->at(i);

            tfloat dh_sum = 0.0;
            tfloat mdh = 0.0;
            std::list<std::tuple<size_t, tfloat>> deposition_nodes;

            for (auto j : this->m_graph->get_connected(i))
            {
                const auto jh = this->m_heights->at(j);
                const auto dh = ih - jh; // delta height
                if (dh <= 0.0)
                {
                    continue;
                }

                if (dh > mdh)
                {
                    mdh = dh;
                }

                const auto& pi = this->m_points->at(i);
                const auto& pj = this->m_points->at(j);
                const auto dist = glm::distance(pi, pj);
                const auto slope = dh / dist;
                if (slope >= this->m_talus_slope)
                {
                    dh_sum += dh;
                    deposition_nodes.push_back(std::make_tuple(j, dh));
                }
            }

            if (deposition_nodes.size() < 1)
            {
                continue;
            }

            const auto deposit_amount = mdh / 2.0;
            this->m_heights->at(i) -= deposit_amount;

            for (auto [j, dh] : deposition_nodes)
            {
                this->m_heights->at(j) += deposit_amount * (dh / dh_sum);
            }

            changed = true;
        }
    }
}
