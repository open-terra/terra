#pragma once

#include <string>

#include "../types/triangle.hpp"

namespace terra::io
{
    void write_obj(const std::string& filepath,
                   const std::vector<terra::vec3>& points,
                   const std::vector<terra::triangle>& triangles);
}