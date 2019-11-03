#include <cmath>
#include <cstdint>
#include <iterator>
#include <iostream>
#include <vector>

#include <catch2/catch.hpp>

#include "terra/types/vector_2.hpp"
#include "terra/delaunator.hpp"

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
static const std::vector<Terra::Vector2> vertices =
{
    { 3.0, 6.0 }, { 4.0, 6.0 },
    { 1.0, 5.0 }, { 2.0, 5.0 }, { 5.0, 5.0 }, { 6.0, 5.0 },
    { 0.0, 4.0 }, { 3.0, 4.0 }, { 4.0, 4.0 }, { 7.0, 4.0 },
    { 1.0, 3.0 }, { 2.0, 3.0 }, { 5.0, 3.0 }, { 6.0, 3.0 },
    { 0.0, 2.0 }, { 3.0, 2.0 }, { 4.0, 2.0 }, { 7.0, 2.0 },
    { 1.0, 1.0 }, { 2.0, 1.0 }, { 5.0, 1.0 }, { 6.0, 1.0 },
    { 3.0, 0.0 }, { 4.0, 0.0 },
};

TEST_CASE("can triangulate correctly", "[Delaunator]")
{
    Terra::Delaunator delaunator(vertices);
    REQUIRE(delaunator.triangles.size() > 0);
}