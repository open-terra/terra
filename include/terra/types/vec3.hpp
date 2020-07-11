#pragma once

#define GLM_FORCE_XYZW_ONLY
#include <glm/vec3.hpp>

#include "vec2.hpp"

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

    constexpr terra::vec3 make_vec3(tfloat x, tfloat y, tfloat z)
    {
        return terra::vec3(x, y, z);
    }

    constexpr terra::vec3 make_vec3(const vec2& p, tfloat z)
    {
        return terra::vec3(p.x, p.y, z);
    }
} // namespace terra