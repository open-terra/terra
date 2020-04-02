#pragma once

#include <vector>

#include "../base_types.hpp"
#include "triangle.hpp"
#include "vec2.hpp"

namespace terra
{
    struct polygon
    {
        std::vector<terra::vec2> vertices;

        polygon& operator=(polygon&& o) noexcept = default;
        polygon(polygon&& o) noexcept = default;

        polygon() : vertices()
        {
        }

        explicit polygon(const std::vector<terra::vec2>& vertices) :
            vertices(vertices)
        {
        }

        tfloat area(const terra::vec2& centre) const
        {
            tfloat area = terra::triangle::area(centre,
                                                this->vertices.front(),
                                                this->vertices.back());
            for (size_t i = 1; i < this->vertices.size(); ++i)
            {
                const auto& v0 = this->vertices[i - 1];
                const auto& v1 = this->vertices[i];

                area += terra::triangle::area(centre, v0, v1);
            }

            return area;
        }
    };
}