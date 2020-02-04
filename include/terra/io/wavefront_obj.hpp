#pragma once

#include <string>
#include <vector>

#include "../types/triangle.hpp"
#include "../types/vec3.hpp"

namespace terra::io::obj
{
    void write_obj(const std::string filepath,
                   const std::vector<terra::vec3>& points,
                   const std::vector<terra::triangle>& triangles);
}