#include "terra/thermal_erosion.hpp"

terra::thermal_erosion::thermal_erosion() :
    m_points(nullptr),
    m_heights(nullptr),
    m_graph(nullptr),
    m_talus_slope(0.0)
{
}

terra::thermal_erosion::thermal_erosion(const std::span<terra::vec2>& points,
                                        std::span<tfloat>& heights,
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
            const auto ih = this->m_heights->data()[i];

            tfloat dh_sum = 0.0;
            tfloat mdh = 0.0;
            std::list<std::tuple<size_t, tfloat>> deposition_nodes;

            for (auto j : this->m_graph->get_connected(i))
            {
                const auto jh = this->m_heights->data()[j];
                const auto dh = ih - jh; // delta height
                if (dh <= 0.0)
                {
                    continue;
                }

                if (dh > mdh)
                {
                    mdh = dh;
                }

                const auto& pi = this->m_points->data()[i];
                const auto& pj = this->m_points->data()[j];
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
            this->m_heights->data()[i] -= deposit_amount;

            for (auto [j, dh] : deposition_nodes)
            {
                this->m_heights->data()[j] += deposit_amount * (dh / dh_sum);
            }

            changed = true;
        }
    }
}