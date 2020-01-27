#pragma once

#include <string>
#include <vector>

#include "../types/bitmap.hpp"

namespace terra::io::tiff
{
    terra::bitmap load_tiff(const std::string filepath);
}