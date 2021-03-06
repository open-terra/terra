#pragma once

#define GLM_FORCE_XYZW_ONLY
#include <glm/vec2.hpp>

#include "../terra_config.hpp"

namespace terra
{
#ifdef TERRA_USE_DOUBLE
    typedef glm::dvec2 vec2;
#else
    typedef glm::vec2 vec2;
#endif

    constexpr void swap(vec2& l, vec2& r)
    {
        auto c = l;
        l = r;
        r = c;
    }

    constexpr terra::vec2 make_vec2(tfloat x, tfloat y)
    {
        return terra::vec2(x, y);
    }

    template<class T>
    constexpr terra::vec2 make_vec2(const T& p)
    {
        return terra::vec2(p.x, p.y);
    }
} // namespace terra
