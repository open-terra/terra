#pragma once

#include <string>
#include <vector>

#include "../types/bitmap.hpp"

namespace terra::io::tiff
{
    template<typename T>
    terra::bitmap<T> load_tiff(const std::string filepath);
}