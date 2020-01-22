#pragma once

#include "../utils/template_helpers.hpp"
#include "detail/graph_t.hpp"

namespace terra
{
    typedef detail::graph_t<graph_type::undirected> undirected_graph;

    template<>
    undirected_graph::graph_t() : nodes(), edges()
    {
    }

    template<>
    undirected_graph::graph_t(size_t vertex_count, size_t max_edges) :
        nodes(vertex_count), edges()
    {
        // TODO still need to free this memory later on somewhere
        this->edges.reserve(max_edges);
    }

    template<>
    undirected_graph::graph_t(size_t node_count,
                            const std::vector<size_t>& triangles) :
        nodes(node_count), edges()
    {
        this->edges.reserve(triangles.size());

        for (size_t i = 0; i < triangles.size(); i += 3)
        {
            const size_t p0 = i;
            const size_t p1 = i + 1;
            const size_t p2 = i + 2;

            const size_t v0 = triangles[p0];
            const size_t v1 = triangles[p1];
            const size_t v2 = triangles[p2];

            this->add_triangle(v0, v1, v2);
        }

        this->edges.shrink_to_fit();
    }

    template<>
    void undirected_graph::add_edge(const undirected_graph::edge& edge)
    {
        if (edge.v0 != edge.v1 && !(this->is_edge_duplicate(edge)))
        {
            const size_t edge_count = this->edges.size();
            this->nodes[edge.v0].push_back(edge_count);
            this->nodes[edge.v1].push_back(edge_count);
            this->edges.push_back(edge);
        }
    }

    template<>
    void undirected_graph::add_edge(size_t v0, size_t v1)
    {
        this->add_edge({v0, v1});
    }

    template<>
    void undirected_graph::add_triangle(size_t v0, size_t v1, size_t v2)
    {
        this->add_edge({v0, v1});
        this->add_edge({v1, v2});
        this->add_edge({v2, v0});
    }

    template<>
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

    template<>
    size_t undirected_graph::num_edges() const
    {
        return this->edges.size();
    }

    template<>
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
} // namespace terra
