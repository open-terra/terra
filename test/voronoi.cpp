#include <cmath>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <span>
#include <vector>

#include <catch2/catch.hpp>

#include "terra/voronoi.hpp"
#include "terra/types/dynarray.hpp"
#include "terra/types/vec2.hpp"

/*
A shifted honey comb to be partitioned
      __
   __/  \__
  /  \__/  \
  \__/  \__/
  /  \__/  \
  \__/  \__/
     \__/
*/
static std::array<terra::vec2, 26> vertices = {
    terra::vec2(2.568f,  6.288f), terra::vec2(4.205f,  5.642f), 
    terra::vec2(0.624f,  5.343f), terra::vec2(1.874f,  4.937f), terra::vec2(4.693f, 4.841f), terra::vec2(6.008f, 5.022f), 
    terra::vec2(0.347f,  4.065f), terra::vec2(3.363f,  3.721f), terra::vec2(3.752f, 4.135f), terra::vec2(6.505f, 3.656f), 
    terra::vec2(1.366f,  3.360f), terra::vec2(1.648f,  3.204f), terra::vec2(5.431f, 2.907f), terra::vec2(6.461f, 2.706f), 
    terra::vec2(0.460f,  2.472f), terra::vec2(2.816f,  1.578f), terra::vec2(3.947f, 1.794f), terra::vec2(7.217f, 1.567f), 
    terra::vec2(0.726f,  0.773f), terra::vec2(1.858f,  1.037f), terra::vec2(4.944f, 1.459f), terra::vec2(5.845f, 0.788f), 
    terra::vec2(3.090f, -0.432f), terra::vec2(4.166f, -0.119f)
};

TEST_CASE("can partition correctly", "[delaunator]")
{
    terra::dynarray<terra::polygon> cells(vertices.size());
    terra::voronoi v;
    v.generate(std::span(vertices.data(), vertices.size()),
               {-1.0, 8.0, -1.0, 8.0},
               std::span(cells));

    REQUIRE(v.num_cells() == vertices.size());
}