#include "terra/types/undirectional_edge.hpp"

using namespace terra;

constexpr undirectional_edge::undirectional_edge() : v0(0), v1(0)
{
}

constexpr undirectional_edge::undirectional_edge(size_t v0, size_t v1) : v0(v0), v1(v1)
{
}

constexpr std::array<size_t, 2ul> undirectional_edge::get_vertex() const
{
    return { this->v0, this->v1 };
}

constexpr bool operator==(const undirectional_edge& l, const undirectional_edge& r)
{
    return (l.v0 == r.v0 && l.v1 == r.v1) ||
           (l.v0 == r.v1 && l.v1 == r.v0);
}