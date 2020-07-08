#include "terra/thermal_erosion.hpp"

terra::thermal_erosion::thermal_erosion() :
    m_points(),
    m_heights(),
    m_graph(nullptr),
    m_talus_slope(0.0)
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
        for (size_t i = 0; i < this->m_points.size(); ++i)
        {
            const auto ih = this->m_heights[i];

            tfloat dh_sum = 0.0;
            tfloat mdh = 0.0;
            std::list<std::tuple<size_t, tfloat>> deposition_nodes;

            for (auto j : this->m_graph->get_connected(i))
            {
                const auto jh = this->m_heights[j];
                const auto dh = ih - jh; // delta height
                if (dh <= 0.0)
                {
                    continue;
                }

                if (dh > mdh)
                {
                    mdh = dh;
                }

                const auto& pi = this->m_points[i];
                const auto& pj = this->m_points[j];
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
            this->m_heights[i] -= deposit_amount;

            for (auto [j, dh] : deposition_nodes)
            {
                this->m_heights[j] += deposit_amount * (dh / dh_sum);
            }

            changed = true;
        }
    }
}