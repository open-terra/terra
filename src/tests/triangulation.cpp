#include <cmath>
#include <cstdint>
#include <iterator>
#include <iostream>
#include <vector>

#include "../types/vector_2.hpp"

#include "../delaunator.hpp"

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

int main(int argc, char* argv[])
{
    Terra::Delaunator delaunator(vertices);
    std::cout << "tri count:" << delaunator.triangles.size() << std::endl;
    for (int i = 0; i + 2 < delaunator.triangles.size(); i += 3)
    {
        std::cout << "tri: { " << delaunator.triangles[i] << ", " << delaunator.triangles[i + 1] << ", " << delaunator.triangles[i + 2] << " }";
    }
}