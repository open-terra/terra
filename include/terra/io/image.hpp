#pragma once

#include <string_view>
#include <vector>

#include "../types/bitmap.hpp"

namespace terra::io
{
    terra::bitmap load_image(const std::string_view& filepath);
    void write_image(const std::string_view& filepath,
                     const terra::bitmap& image);
}