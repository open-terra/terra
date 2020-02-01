#pragma once

#include <vector>

#include "vec2.hpp"

namespace terra
{
    struct polygon
    {
        size_t centre_idx;
        std::vector<terra::vec2> vertices;

        polygon() : centre_idx(0), vertices()
        {
        }

        polygon(size_t centre_idx, std::vector<terra::vec2>& vertices) :
            centre_idx(centre_idx), vertices(vertices)
        {
        }
    };
}