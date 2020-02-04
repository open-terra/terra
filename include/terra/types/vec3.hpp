#pragma once

#include <glm/vec3.hpp>

namespace terra
{
#ifdef TERRA_USE_DOUBLE
    typedef glm::dvec3 vec3;
#else
    typedef glm::vec3 vec3;
#endif

    constexpr void swap(vec3& l, vec3& r)
    {
        auto c = l;
        l = r;
        r = c;
    }
} // namespace terra