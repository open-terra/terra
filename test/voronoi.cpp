#include <cmath>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <vector>

#include <catch2/catch.hpp>

#include "terra/voronoi.hpp"
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
static const std::vector<terra::vec2> vertices = {
    { 2.568,  6.288}, {4.205,  5.642}, 
    { 0.624,  5.343}, {1.874,  4.937}, {4.693, 4.841}, {6.008, 5.022}, 
    { 0.347,  4.065}, {3.363,  3.721}, {3.752, 4.135}, {6.505, 3.656}, 
    { 1.366,  3.360}, {1.648,  3.204}, {5.431, 2.907}, {6.461, 2.706}, 
    {-0.460,  2.472}, {2.816,  1.578}, {3.947, 1.794}, {7.217, 1.567}, 
    { 0.726,  0.773}, {1.858,  1.037}, {4.944, 1.459}, {5.845, 0.788}, 
    { 3.090, -0.432}, {4.166, -0.119}
};

TEST_CASE("can partition correctly", "[delaunator]")
{
    terra::dynarray<terra::polygon> cells(vertices.size());
    terra::voronoi v;
    v.generate(vertices, {-1.0, 8.0, -1.0, 8.0}, cells);

    REQUIRE(v.num_cells() == vertices.size());
}