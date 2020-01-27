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
                              const std::vector<terra::triangle>& triangles) :
        nodes(node_count), edges()
    {
        this->edges.reserve(triangles.size());

        for (const auto& tri : triangles)
        {
            this->add_triangle(tri);
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
    void undirected_graph::add_triangle(const terra::triangle& tri)
    {
        this->add_edge({tri.v0, tri.v1});
        this->add_edge({tri.v1, tri.v2});
        this->add_edge({tri.v2, tri.v0});
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
