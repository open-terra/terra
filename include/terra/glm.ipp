#pragma once

#include "glm.hpp"

#include <glm/gtx/norm.hpp>

template<class Vec>
static inline typename Vec::value_type
terra::glm_vec2_conf_t<Vec>::get_x(const point_type& p)
{
    return p.x;
}

template<class Vec>
static inline typename Vec::value_type
terra::glm_vec2_conf_t<Vec>::get_y(const point_type& p)
{
    return p.y;
}

template<class Vec>
static inline typename Vec::value_type
terra::glm_vec2_conf_t<Vec>::get_dist2(const point_type& p0,
                                       const point_type& p1)
{
    return glm::distance2(p0, p1);
}
