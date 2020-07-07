#include <cmath>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <vector>

#include <catch2/catch.hpp>

#include "terra/delaunator.hpp"
#include "terra/types/vec2.hpp"

/*
Honey comb to be triangulated
      __
   __/  \__
  /  \__/  \
  \__/  \__/
  /  \__/  \
  \__/  \__/
     \__/
*/
static const std::vector<terra::vec2> vertices = {
    {3.0, 6.0}, {4.0, 6.0}, {1.0, 5.0}, {2.0, 5.0}, {5.0, 5.0}, {6.0, 5.0},
    {0.0, 4.0}, {3.0, 4.0}, {4.0, 4.0}, {7.0, 4.0}, {1.0, 3.0}, {2.0, 3.0},
    {5.0, 3.0}, {6.0, 3.0}, {0.0, 2.0}, {3.0, 2.0}, {4.0, 2.0}, {7.0, 2.0},
    {1.0, 1.0}, {2.0, 1.0}, {5.0, 1.0}, {6.0, 1.0}, {3.0, 0.0}, {4.0, 0.0},
};

TEST_CASE("can triangulate correctly", "[delaunator]")
{
    terra::delaunator d;
    d.triangulate(std::span(vertices));
    REQUIRE(d.triangles.size() > 0);
}