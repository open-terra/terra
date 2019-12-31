#pragma once

#include <array>
#include <cstddef>

namespace terra
{
    struct undirected_edge
    {
    public:
        size_t v0, v1;

        constexpr undirected_edge() : v0(0), v1(0)
        {
        }

        constexpr undirected_edge(size_t v0, size_t v1) : v0(v0), v1(v1)
        {
        }

        ~undirected_edge()
        {
        }

        constexpr std::array<size_t, 2ul> get_vertex() const
        {
            return { this->v0, this->v1 };
        }

        constexpr void sort_vertex()
        {
            if (this->v0 > this->v1)
            {
                std::swap(this->v0, this->v1);
            }
        }
    };
}

constexpr bool operator==(const terra::undirected_edge& l,
                          const terra::undirected_edge& r)
{
    return (l.v0 == r.v0 && l.v1 == r.v1) ||
           (l.v0 == r.v1 && l.v1 == r.v0);
}