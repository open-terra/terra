#include "terra/types/undirectional_graph.hpp"

#include <array>

using namespace terra;

undirectional_graph::undirectional_graph() : data()
{
}

undirectional_graph::undirectional_graph(size_t vertex_count) : data(vertex_count)
{
}

void undirectional_graph::add_edge(const undirectional_graph::edge& edge)
{
    if (!(this->is_edge_duplicate(edge)))
    {
        this->data[edge.v0].push_back(edge.v1);
        this->data[edge.v1].push_back(edge.v0);
    }
}

void undirectional_graph::add_edge(size_t v0, size_t v1)
{
    this->add_edge({ v0, v1 });
}

std::list<size_t> undirectional_graph::get_connected(size_t i) const
{
    return this->data[i];
}

bool undirectional_graph::is_edge_duplicate(const undirectional_graph::edge& edge)
{
    //TODO theoretically this only needs to check one of the vertices as a
    // connection should always have been added to both.
    for (const auto v : this->data[edge.v0])
    {
        if (edge.v1 == v) return true;
    }
    for (const auto v : this->data[edge.v1])
    {
        if (edge.v0 == v) return true;
    }

    return false;
}
