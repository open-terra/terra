#pragma once

#include <list>
#include <vector>

#include "undirected_edge.hpp"

namespace terra
{
    class undirected_graph
    {
    public:
        typedef undirected_edge edge;

        undirected_graph();
        undirected_graph(size_t node_count, size_t max_edges);

        void add_edge(const edge& edge);
        void add_edge(size_t v0, size_t v1);

        std::vector<size_t> get_connected(size_t i) const;

        size_t num_edges() const;

    private:
        size_t edge_count;
        std::vector<std::list<size_t>> nodes;
        std::vector<edge> edges;

        bool is_edge_duplicate(const edge& edge) const;
    };
}