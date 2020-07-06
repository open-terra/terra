#include <cmath>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <vector>

#include <catch2/catch.hpp>

#include "terra/delaunator.hpp"
#include "terra/math/floor.hpp"
#include "terra/noise/ridged_noise.hpp"
#include "terra/poisson_disc_sampler.hpp"
#include "terra/thermal_erosion.hpp"
#include "terra/types/dynarray.hpp"
#include "terra/types/flow_graph.hpp"
#include "terra/types/vec2.hpp"

TEST_CASE("can simulate thermal erosion", "[erosion]")
{
    terra::poisson_disc_sampler p;
    auto points = p.sample(512, 512, 1.0, 30);

    terra::dynarray<terra::triangle> tris;
    {
        terra::delaunator d;
        d.triangulate(points);

        tris = terra::dynarray<terra::triangle>(d.triangles.size() / 3);
        for (size_t i = 0; i < tris.size(); ++i)
        {
            size_t index = i * 3;
            const size_t p0 = index;
            const size_t p1 = index + 1;
            const size_t p2 = index + 2;

            const size_t v0 = d.triangles[p0];
            const size_t v1 = d.triangles[p1];
            const size_t v2 = d.triangles[p2];

            tris[i] = terra::triangle(v0, v1, v2);
        }
    }

    terra::dynarray<tfloat> heights(points.size());
    {
        terra::noise::ridged_noise n(1337);
        n.set_octaves(6);
        auto noise = n.noise(123, 543, 1, 512, 512, 1, 0.125);

        for (size_t i = 0; i < points.size(); ++i)
        {
            const auto p = points[i];
            auto x = terra::math::floor<size_t>(p.x);
            auto y = terra::math::floor<size_t>(p.y);
            heights[i] = noise[(y * 512) + x];
        }
    }

    terra::undirected_graph graph(points.size(), tris);
    terra::thermal_erosion e(points, std::span(heights), graph, 40.0);
    e.update();

    bool valid = true;
    for (auto h : heights)
    {
        if (std::isnan(h) || std::isinf(h))
        {
            valid = false;
            break;
        }
    }

    REQUIRE(valid);
}