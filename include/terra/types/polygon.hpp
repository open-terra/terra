#pragma once

#include <vector>

#include "../base_types.hpp"
#include "vec2.hpp"

namespace terra
{
    struct polygon
    {
        std::vector<terra::vec2> vertices;

        polygon() : vertices()
        {
        }

        polygon(const std::vector<terra::vec2>& vertices) :
            vertices(vertices)
        {
        }

        tfloat area(const terra::vec2& centre) const
        {
            tfloat area = area_of_tri(centre,
                                      this->vertices.front(),
                                      this->vertices.back());
            for (size_t i = 1; i < this->vertices.size(); ++i)
            {
                const auto& v0 = this->vertices[i - 1];
                const auto& v1 = this->vertices[i];

                area += area_of_tri(centre, v0, v1);
            }

            return area;
        }

    private:
        inline tfloat area_of_tri(const terra::vec2& a, const terra::vec2& b, const terra::vec2& c) const
        {
            tfloat area2 = (a.x * (b.y - c.y)) +
                           (b.x * (c.y - a.y)) +
                           (c.x * (a.y - b.y));

            return area2 / 2;
        }
    };
}