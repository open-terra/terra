#pragma once

#include <glm/vec3.hpp>

namespace terra
{
    typedef glm::vec3 vec3;

    constexpr void swap(vec3& l, vec3& r)
    {
        auto c = l;
        l = r;
        r = c;
    }
}