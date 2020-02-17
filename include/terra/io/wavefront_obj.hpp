#pragma once

#include <string>

#include "../types/dynarray.hpp"
#include "../types/triangle.hpp"
#include "../types/vec3.hpp"

namespace terra::io::obj
{
    void write_obj(const std::string& filepath,
                   const terra::dynarray<terra::vec3>& points,
                   const terra::dynarray<terra::triangle>& triangles);
}