#include "terra/types/undirected_graph.hpp"

using namespace terra;

undirected_graph::undirected_graph() : edge_count(0), nodes(), edges()
{
}

undirected_graph::undirected_graph(size_t vertex_count, size_t max_edges) :
    edge_count(0), nodes(vertex_count), edges(max_edges)
{
    // TODO probably reserve edges here instead of setting the initial size

    // TODO also need to shrink edges at some point or find a better solution
    // for storing edges, maybe use a list again, or have a seperate constructor
    // class
}

void undirected_graph::add_edge(const undirected_graph::edge& edge)
{
    if (edge.v0 != edge.v1 && !(this->is_edge_duplicate(edge)))
    {
        this->nodes[edge.v0].push_back(this->edge_count);
        this->nodes[edge.v1].push_back(this->edge_count);
        this->edges[this->edge_count] = edge;
        ++(this->edge_count);
    }
}

void undirected_graph::add_edge(size_t v0, size_t v1)
{
    this->add_edge({v0, v1});
}

std::vector<size_t> undirected_graph::get_connected(size_t i) const
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

size_t undirected_graph::num_edges() const
{
    return this->edge_count;
}

bool undirected_graph::is_edge_duplicate(
    const undirected_graph::edge& edge) const
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
