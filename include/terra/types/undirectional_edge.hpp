#pragma once

#include <array>
#include <cstddef>

namespace terra
{
    struct undirectional_edge
    {
        size_t v0, v1;

        constexpr undirectional_edge();
        constexpr undirectional_edge(size_t v0, size_t v1);

        constexpr std::array<size_t, 2ul> get_vertex() const;
    };
}

constexpr bool operator==(const terra::undirectional_edge&, const terra::undirectional_edge&);
