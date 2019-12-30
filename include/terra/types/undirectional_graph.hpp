#pragma once

#include <list>
#include <vector>

#include "undirectional_edge.hpp"

namespace terra
{
    class undirectional_graph
    {
    public:
        typedef undirectional_edge edge;

        undirectional_graph();
        undirectional_graph(size_t);

        void add_edge(const edge& edge);
        void add_edge(size_t v0, size_t v1);

    private:
        std::vector<std::list<size_t>> data;

        bool is_edge_duplicate(const edge& edge);
    };
}