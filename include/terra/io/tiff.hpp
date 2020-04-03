#pragma once

#include <string_view>
#include <vector>

#include "../types/bitmap.hpp"

namespace terra::io::tiff
{
    terra::bitmap load_tiff(const std::string_view& filepath);
}