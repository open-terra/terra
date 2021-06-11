#include "graph_t.hpp"

template<terra::graph_type T>
inline terra::detail::graph_t<T>::graph_t() : m_nodes(0), m_edges()
{
}

template<terra::graph_type T>
inline terra::detail::graph_t<T>::graph_t(size_t node_count)
    : m_nodes(node_count), m_edges()
{
}

template<terra::graph_type T>
inline void terra::detail::graph_t<T>::add_edge(const graph_t<T>::edge_type& edge)
{
    if (edge.v0 != edge.v1 && !(this->is_edge_duplicate(edge)))
    {
        const size_t edge_count = this->m_edges.size();
        this->m_nodes[edge.v0].push_back(edge_count);
        this->m_nodes[edge.v1].push_back(edge_count);
        this->m_edges.push_back(edge);
    }
}

template<terra::graph_type T>
inline void terra::detail::graph_t<T>::add_edge(size_t v0, size_t v1)
{
    this->add_edge({ v0, v1 });
}

template<terra::graph_type T>
inline void terra::detail::graph_t<T>::add_triangle(size_t v0, size_t v1, size_t v2)
{
    this->add_edge(v0, v1);
    this->add_edge(v1, v2);
    this->add_edge(v2, v0);
}

template<terra::graph_type T>
inline void terra::detail::graph_t<T>::shrink_to_fit()
{
    this->m_nodes.shrink_to_fit();
    this->m_edges.shrink_to_fit();
}

template<terra::graph_type T>
inline terra::detail::graph_edge_t<T>& terra::detail::graph_t<T>::get_edge(size_t i)
{
    return this->m_edges[i];
}

template<terra::graph_type T>
inline const terra::detail::graph_edge_t<T>& terra::detail::graph_t<T>::get_edge(size_t i) const
{
    return this->m_edges[i];
}

template<terra::graph_type T>
inline terra::detail::graph_t<T>::node_type& terra::detail::graph_t<T>::get_node(size_t i)
{
    return this->m_nodes[i];
}

template<terra::graph_type T>
inline const terra::detail::graph_t<T>::node_type& terra::detail::graph_t<T>::get_node(size_t i) const
{
    return this->m_nodes[i];
}

template<terra::graph_type T>
inline std::vector<size_t> terra::detail::graph_t<T>::get_connected(size_t i) const
{
    std::vector<size_t> connected;
    connected.reserve(this->m_nodes[i].size());

    for (const auto iedge : this->m_nodes[i])
    {
        const auto& edge = this->m_edges[iedge];
        const auto  con_node = edge.v0 == i ? edge.v1 : edge.v0;
        connected.push_back(con_node);
    }

    return connected;
}

template<terra::graph_type T>
inline size_t terra::detail::graph_t<T>::num_edges() const
{
    return this->m_edges.size();
}

template<terra::graph_type T>
inline bool terra::detail::graph_t<T>::is_edge_duplicate(const graph_t<T>::edge_type& edge) const
{
    for (const auto iedge : this->m_nodes[edge.v0])
    {
        const auto& e = this->m_edges[iedge];
        if (e == edge)
        {
            return true;
        }
    }

    return false;
}