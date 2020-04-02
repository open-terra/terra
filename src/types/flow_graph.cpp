#include "terra/types/flow_graph.hpp"

#include <algorithm>

using namespace terra;

flow_graph::flow_graph() :
    drainage_areas(0), flow(0), lakes(), sorted_nodes(0), graph(nullptr),
    areas(nullptr), heights(nullptr)
{
}

flow_graph::flow_graph(size_t node_count,
                       const terra::undirected_graph& graph,
                       const terra::dynarray<tfloat>& areas,
                       const terra::dynarray<tfloat>& heights) :
    drainage_areas(node_count), flow(node_count), lakes(), 
    sorted_nodes(node_count), graph(&graph), areas(&areas), heights(&heights)
{
    for (size_t i = 0; i < node_count; ++i)
    {
        sorted_nodes[i] = i;
    }
}

void flow_graph::update()
{
    this->update_sort_nodes();
    this->update_flow();
    this->update_drainage_areas();
}

const terra::undirected_graph& flow_graph::get_graph() const
{
    return *(this->graph);
}

struct compare
{
    const terra::dynarray<tfloat>* heights;

    bool operator()(const size_t i, const size_t j)
    {
        const tfloat ih = (*this->heights)[i];
        const tfloat jh = (*this->heights)[j];

        return ih < jh;
    }
};

void flow_graph::update_sort_nodes()
{
    std::sort(this->sorted_nodes.begin(),
              this->sorted_nodes.end(),
              compare{this->heights});
}

void flow_graph::update_drainage_areas()
{
    std::fill(this->drainage_areas.begin(), this->drainage_areas.end(), 0.0);

    for (const auto node : this->sorted_nodes)
    {
        tfloat drainage_area = this->areas->at(node);
        const tfloat nh = this->heights->at(node);

        for (const auto con_node : this->graph->get_connected(node))
        {
            const tfloat ch = this->heights->at(con_node);

            if (ch > nh)
            {
                drainage_area += this->drainage_areas[con_node];
            }
        }

        this->drainage_areas[node] = drainage_area;
    }
}

void flow_graph::update_flow()
{
    for (const auto node : this->sorted_nodes)
    {
        const tfloat nh = this->heights->at(node);
        auto min_node = std::make_pair(flow_graph::node_lake, nh);

        for (const auto con_node : this->graph->get_connected(node))
        {
            const tfloat con_height = this->heights->at(con_node);
            if (con_height < min_node.second)
            {
                min_node = std::make_pair(con_node, con_height);
            }
        }

        if (min_node.first == flow_graph::node_lake)
        {
            this->lakes.push_back(min_node.first);
        }

        this->flow[node] = min_node.first;
    }
}
