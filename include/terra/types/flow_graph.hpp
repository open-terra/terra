#pragma once

#include <cstddef>
#include <limits>
#include <list>
#include <utility>

#include "terra/types/undirected_graph.hpp"

namespace terra
{
    class flow_graph
    {
    public:
        static constexpr size_t node_lake =
            std::numeric_limits<size_t>::max() - 1ull;
        static constexpr size_t node_external =
            std::numeric_limits<size_t>::max();

        std::vector<size_t> flow;
        std::list<size_t> lakes;
        std::vector<tfloat> drainage_areas;
        std::vector<size_t> sorted_nodes;

    private:
        const terra::undirected_graph* graph;
        const std::vector<tfloat>* areas;
        const std::vector<tfloat>* heights;

    public:
        flow_graph();
        flow_graph(size_t node_count,
                   const terra::undirected_graph& graph,
                   const std::vector<tfloat>& areas,
                   const std::vector<tfloat>& heights);

        void update();

        const terra::undirected_graph& get_graph() const;

    private:
        void init_sort_nodes();
        void init_flow();

        void update_sort_nodes();
        void update_drainage_areas();
        void update_flow();
    };
} // namespace terra
