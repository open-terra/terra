#include "terra/delaunay.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

bool terra::delaunay::circle::point_within(const terra::vec2& p)
{
    return glm::distance2(this->centre, p) <= this->r2;
}
