#include <cmath>
#include <cstdint>
#include <iterator>
#include <iostream>
#include <vector>

#include "../types/vector_2.hpp"

#include "../delaunator.hpp"

static std::vector<Terra::Vector2> points = 
{
    
};

static std::vector<size_t> tris;

int main(int argc, char* argv[])
{
    Terra::Delaunator delaunator(points);
    for (int i = 0; i + 2 < delaunator.triangles.size(); i += 3)
    {
        std::cout << "tri: { " << delaunator.triangles[i] << ", " << delaunator.triangles[i + 1] << ", " << delaunator.triangles[i + 2] << " }";
    }
}