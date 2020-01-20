#include "terra/io/wavefront_obj.hpp"

#include <iostream>
#include <fstream>

using namespace terra::io::obj;

void write_obj(const std::string filepath,
               const std::vector<terra::vec2>& points,
               const std::vector<terra::triangle>& triangles)
{
    std::ofstream obj_file;
    obj_file.open(filepath);

    for (const auto& p : points)
    {
        obj_file << "v " << p.x << " " << p.y << " " << 0.0 << std::endl;
    }

    for (const auto& tri : triangles)
    {
        obj_file << "f " << tri.v0 << " " << tri.v1 << " " << tri.v2 << std::endl;
    }

    obj_file.close();
}