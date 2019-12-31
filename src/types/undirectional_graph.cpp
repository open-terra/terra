#include "terra/types/undirectional_graph.hpp"

#include <array>

using namespace terra;

undirectional_graph::undirectional_graph() : nodes(), edges()
{
}

undirectional_graph::undirectional_graph(size_t vertex_count, size_t max_edges) :
    nodes(vertex_count),
    edges(max_edges)
{
}

void undirectional_graph::add_edge(const undirectional_graph::edge& edge)
{
    if (edge.v0 != edge.v1 && !(this->is_edge_duplicate(edge)))
    {
        this->nodes[edge.v0].push_back(this->edge_count);
        this->nodes[edge.v1].push_back(this->edge_count);
        this->edges[this->edge_count] = edge;
        ++(this->edge_count);
    }
}

void undirectional_graph::add_edge(size_t v0, size_t v1)
{
    this->add_edge({ v0, v1 });
}

std::vector<size_t> undirectional_graph::get_connected(size_t i) const
{
    std::vector<size_t> connected;
    connected.reserve(this->nodes[i].size());

    for (const auto iedge : this->nodes[i])
    {
        const auto& edge = this->edges[iedge];
        size_t con_node = edge.v0 == i ? edge.v1 : edge.v0;
        connected.push_back(con_node);
    }

    return connected;
}

size_t undirectional_graph::num_edges() const
{
    return this->edge_count;
}

bool undirectional_graph::is_edge_duplicate(const undirectional_graph::edge& edge) const
{
    for (const auto iedge : this->nodes[edge.v0])
    {
        const auto& e = this->edges[iedge];
        if (e == edge)
        {
            return true;
        }
    }

    return false;
}
