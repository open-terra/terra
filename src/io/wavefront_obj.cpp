#include "terra/io/wavefront_obj.hpp"

#include <iostream>
#include <fstream>

void terra::io::obj::write_obj(const std::string filepath,
                               const terra::dynarray<terra::vec3>& points,
                               const terra::dynarray<terra::triangle>& triangles)
{
    std::ofstream obj_file;
    obj_file.open(filepath);

    for (const auto& p : points)
    {
        obj_file << "v " << p.x << " " << p.y << " " << p.z << std::endl;
    }

    for (const auto& tri : triangles)
    {
        const size_t i_0 = tri.v0 + 1;
        const size_t i_1 = tri.v1 + 1;
        const size_t i_2 = tri.v2 + 1;
        obj_file << "f " << i_0 << " " << i_1 << " " << i_2 << std::endl;
    }

    obj_file.close();
}