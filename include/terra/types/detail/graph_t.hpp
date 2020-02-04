#pragma once

#include <list>
#include <vector>

#include "../../enums/graph_type.hpp"
#include "../dynarray.hpp"
#include "../triangle.hpp"
#include "graph_edge_t.hpp"

namespace terra::detail
{
    template<graph_type T>
    class graph_t
    {
    public:
        typedef graph_edge_t<T> edge;

        graph_t();
        graph_t(size_t vertex_count, size_t max_edges);
        template<class Array = terra::dynarray<terra::triangle>>
        graph_t(size_t node_count, const Array& triangles);

        void add_edge(const edge& edge);
        inline void add_edge(size_t v0, size_t v1)
        {
            this->add_edge({v0, v1});
        }

        void add_triangle(const terra::triangle& tri);
        inline void add_triangle(size_t v0, size_t v1, size_t v2)
        {
            this->add_triangle({v0, v1, v2});
        }

        std::vector<size_t> get_connected(size_t i) const;

        size_t num_edges() const;

    private:
        std::vector<std::list<size_t>> nodes;
        std::vector<edge> edges;

        bool is_edge_duplicate(const edge& edge) const;
    };

    template<graph_type T>
    graph_t<T>::graph_t() : nodes(), edges()
    {
    }

    template<graph_type T>
    graph_t<T>::graph_t(size_t vertex_count, size_t max_edges) :
        nodes(vertex_count), edges()
    {
        // TODO still need to free this memory later on somewhere
        this->edges.reserve(max_edges);
    }

    template<graph_type T>
    template<class Array>
    graph_t<T>::graph_t(size_t node_count,
                        const Array& triangles) :
        nodes(node_count), edges()
    {
        this->edges.reserve(triangles.size());

        for (const terra::triangle& tri : triangles)
        {
            this->add_triangle(tri);
        }

        this->edges.shrink_to_fit();
    }

    template<graph_type T>
    void graph_t<T>::add_edge(const graph_t<T>::edge& edge)
    {
        if (edge.v0 != edge.v1 && !(this->is_edge_duplicate(edge)))
        {
            const size_t edge_count = this->edges.size();
            this->nodes[edge.v0].push_back(edge_count);
            this->nodes[edge.v1].push_back(edge_count);
            this->edges.push_back(edge);
        }
    }

    template<graph_type T>
    void graph_t<T>::add_triangle(const terra::triangle& tri)
    {
        this->add_edge({tri.v0, tri.v1});
        this->add_edge({tri.v1, tri.v2});
        this->add_edge({tri.v2, tri.v0});
    }

    template<graph_type T>
    std::vector<size_t> graph_t<T>::get_connected(size_t i) const
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

    template<graph_type T>
    size_t graph_t<T>::num_edges() const
    {
        return this->edges.size();
    }

    template<graph_type T>
    bool graph_t<T>::is_edge_duplicate(const graph_t<T>::edge& edge) const
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
} // namespace terra::detail
