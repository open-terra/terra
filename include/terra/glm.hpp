#pragma once

#include <glm/glm.hpp>

namespace terra
{
    template<class Vec>
    class glm_vec2_conf_t
    {
    public:
        using value_type = typename Vec::value_type;
        using point_type = Vec;

        static inline value_type get_x(const point_type& p);
        static inline value_type get_y(const point_type& p);

        static inline value_type get_dist2(const point_type& p0, const point_type& p1);
    };

    using glm_vec2_config = glm_vec2_conf_t<glm::vec2>;
    using glm_dvec2_config = glm_vec2_conf_t<glm::dvec2>;
}

#include "glm.ipp"
