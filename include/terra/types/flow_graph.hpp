#pragma once

#include <limits>
#include <list>
#include <utility>
#include <vector>

#include "undirected_graph.hpp"

namespace terra
{
    class flow_graph
    {
    public:
        static constexpr size_t node_lake = std::numeric_limits<size_t>::max();

        std::vector<size_t> flow;
        std::list<size_t> lakes;
        std::vector<double> drainage_areas;

    private:
        std::vector<size_t> sorted_nodes;
        const terra::undirected_graph* graph;
        const std::vector<double>* areas;
        const std::vector<double>* heights;

    public:
        flow_graph();
        flow_graph(size_t node_count,
                   const terra::undirected_graph& graph,
                   const std::vector<double>& areas,
                   const std::vector<double>& heights);

        void update();

    private:
        // TODO make flow graph decisions template? needs to be more dynamic,
        // because will allow for multiple methods for flow calculation, for
        // example height & surface hardness, height, shrubs, trees & surface
        // etc. Also shall clean up this class from having as many pointers,
        // out of the object.

        void update_sort_nodes();
        void update_flow();
        void update_drainage_areas();
    };
} // namespace terra
