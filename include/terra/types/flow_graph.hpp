#pragma once

#include <cstddef>
#include <limits>
#include <list>
#include <utility>

#include "../base_types.hpp"
#include "dynarray.hpp"
#include "undirected_graph.hpp"

namespace terra
{
    class flow_graph
    {
    public:
        static constexpr size_t node_lake = std::numeric_limits<size_t>::max();

        terra::dynarray<size_t> flow;
        std::list<size_t> lakes;
        terra::dynarray<tfloat> drainage_areas;
        terra::dynarray<size_t> sorted_nodes;

    private:
        const terra::undirected_graph* graph;
        const terra::dynarray<tfloat>* areas;
        const terra::dynarray<tfloat>* heights;

    public:
        flow_graph();
        flow_graph(size_t node_count,
                   const terra::undirected_graph& graph,
                   const terra::dynarray<tfloat>& areas,
                   const terra::dynarray<tfloat>& heights);

        void update();

    private:
        // TODO make flow graph decisions template? needs to be more dynamic,
        // because will allow for multiple methods for flow calculation, for
        // example height & surface hardness, height, shrubs, trees & surface
        // etc. Also shall clean up this class from having as many pointers,
        // out of the object.

        void update_sort_nodes();
        void update_drainage_areas();
        void update_flow();
    };
} // namespace terra
