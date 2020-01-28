#pragma once

#include <array>
#include <cstddef>

#include "../../enums/graph_type.hpp"

namespace terra::detail
{
    template<graph_type T>
    struct graph_edge_t
    {
    public:
        size_t v0, v1;

        constexpr graph_edge_t() : v0(0), v1(0)
        {
        }

        constexpr graph_edge_t(size_t v0, size_t v1) : v0(v0), v1(v1)
        {
        }

        ~graph_edge_t()
        {
        }

        constexpr std::array<size_t, 2ul> get_vert_idx() const
        {
            return {this->v0, this->v1};
        }

        constexpr void sort_vertex()
        {
            if (this->v0 > this->v1)
            {
                std::swap(this->v0, this->v1);
            }
        }
    };
} // namespace terra::detail

constexpr bool operator==(const terra::detail::graph_edge_t<terra::graph_type::directed>& l,
                          const terra::detail::graph_edge_t<terra::graph_type::directed>& r)
{
    return (l.v0 == r.v0 && l.v1 == r.v1);
}

constexpr bool operator==(const terra::detail::graph_edge_t<terra::graph_type::undirected>& l,
                          const terra::detail::graph_edge_t<terra::graph_type::undirected>& r)
{
    return (l.v0 == r.v0 && l.v1 == r.v1) || (l.v0 == r.v1 && l.v1 == r.v0);
}
