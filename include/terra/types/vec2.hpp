#pragma once

#include <glm/vec2.hpp>

namespace Terra
{
    typedef glm::dvec2 vec2;

    constexpr void swap(vec2& l, vec2& r)
    {
        auto c = l;
        l = r;
        r = c;
    }
}
