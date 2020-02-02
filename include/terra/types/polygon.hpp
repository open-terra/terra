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

        double area(const terra::vec2& centre) const
        {
            double area = area_of_tri(centre,
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
        double area_of_tri(const terra::vec2& a, const terra::vec2& b, const terra::vec2& c) const
        {
            double area2 = (a.x * (b.y - c.y)) +
                           (b.x * (c.y - a.y)) +
                           (c.x * (a.y - b.y));

            return area2 / 2;
        }
    };
}