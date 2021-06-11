#pragma once

#include <list>
#include <vector>

#include "../../enums/graph_type.hpp"
#include "graph_edge_t.hpp"

namespace terra::detail
{
    template<graph_type T>
    class graph_t
    {
    public:
        using edge_type = graph_edge_t<T>;
        using node_type = std::vector<size_t>;

        graph_t();
        graph_t(size_t node_count);

        void add_edge(const edge_type& edge);
        void add_edge(size_t v0, size_t v1);
        void add_triangle(size_t v0, size_t v1, size_t v2);
        void shrink_to_fit();

        edge_type& get_edge(size_t i);
        const edge_type& get_edge(size_t i) const;
        node_type& get_node(size_t i);
        const node_type& get_node(size_t i) const;
        std::vector<size_t> get_connected(size_t i) const;

        size_t num_edges() const;

    private:
        std::vector<node_type> m_nodes;
        std::vector<edge_type>         m_edges;

        bool is_edge_duplicate(const edge_type& edge) const;
    };

} // namespace terra::detail

#include "terra/types/detail/graph_t.ipp"
