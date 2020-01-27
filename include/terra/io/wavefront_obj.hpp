#pragma once

#include <string>
#include <vector>

#include "../types/triangle.hpp"
#include "../types/undirected_edge.hpp"
#include "../types/vec2.hpp"

namespace terra::io::obj
{
    void write_obj(const std::string filepath,
                   const std::vector<terra::vec2>& points,
                   const std::vector<terra::triangle>& tris);
}