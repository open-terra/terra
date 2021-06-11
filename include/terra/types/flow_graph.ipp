#include "terra/types/flow_graph.hpp"

#include <algorithm>
#include <ranges>

flow_graph::flow_graph() :
    drainage_areas(0), flow(0), lakes(), sorted_nodes(0), graph(nullptr),
    areas(nullptr), heights(nullptr)
{
}

flow_graph::flow_graph(size_t node_count,
                       const terra::undirected_graph& graph,
                       const std::vector<tfloat>& areas,
                       const std::vector<tfloat>& heights) :
    flow(node_count), lakes(), drainage_areas(node_count),
    sorted_nodes(node_count), graph(&graph), areas(&areas), heights(&heights)
{
    this->init_sort_nodes();
    this->init_flow();
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

void flow_graph::init_sort_nodes()
{
    for (size_t i = 0; i < this->sorted_nodes.size(); ++i)
    {
        sorted_nodes[i] = i;
    }
}

void flow_graph::init_flow()
{
    std::fill(this->flow.begin(), this->flow.end(), this->node_lake);

    std::vector<uint8_t> edge_table(this->graph->num_edges());
    for (size_t i = 0; i < this->heights->size(); ++i)
    {
        for (const auto e : this->graph->get_node(i))
        {
            ++edge_table[e];
        }
    }

    for (size_t i = 0; i < edge_table.size(); ++i)
    {
        if (edge_table[i] < 2)
        {
            const auto& e = this->graph->get_edge(i);
            this->flow[e.v0] = this->node_external;
            this->flow[e.v1] = this->node_external;
        }
    }
}

struct compare
{
    const std::vector<tfloat>* heights;

    bool operator()(const size_t i, const size_t j)
    {
        const auto ih = this->heights->at(i);
        const auto jh = this->heights->at(j);

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

    for (const auto node : std::ranges::reverse_view{this->sorted_nodes})
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
    for (size_t i = 0; i < this->flow.size(); ++i)
    {
        if (this->flow[i] == this->node_external)
        {
            continue;
        }

        const tfloat nh = this->heights->at(i);
        auto min_node = std::make_pair(flow_graph::node_lake, nh);

        for (const auto con_node : this->graph->get_connected(i))
        {
            const tfloat con_height = this->heights->at(con_node);
            if (con_height < min_node.second)
            {
                min_node = std::make_pair(con_node, con_height);
            }
        }

        if (min_node.first == flow_graph::node_lake)
        {
            this->lakes.push_back(i);
        }

        this->flow[i] = min_node.first;
    }
}
