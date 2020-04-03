#include "terra/io/image.hpp"

#include <iostream>
#include <fstream>

#include "../stb/stb_image.h"
#include "../stb/stb_image_write.h"

terra::bitmap terra::io::load_image(const std::string_view& filepath)
{
    int x = 0, y = 0, channels = 0;
    auto* image_buffer = stbi_load(filepath.data(), &x, &y, &channels, 0);
    if(!image_buffer)
    {
        throw std::runtime_error("failed to read image");
    }

    // Get the width and height
    size_t width = static_cast<size_t>(x);
    size_t height = static_cast<size_t>(y);
    size_t spp = static_cast<size_t>(channels);
    size_t num_pixels = width * height;
    size_t size = num_pixels * channels;
    terra::dynarray<uint8_t> raster(size);
    {
        auto* image_data = reinterpret_cast<uint8_t*>(image_buffer);
        std::copy(image_data, image_data + size, raster.data());
    }

    // Free the buffer
    stbi_image_free(image_buffer);

    return terra::bitmap(width, height, 8, spp, num_pixels, raster);
}

void terra::io::write_image(const std::string_view& filename,
                            const terra::bitmap& image)
{
    stbi_write_png(filename.data(),
                   static_cast<int>(image.get_width()),
                   static_cast<int>(image.get_height()),
                   static_cast<int>(image.get_samples_per_pixel()),
                   image.get<void>(),
                   static_cast<int>(image.get_width() *
                                    image.get_samples_per_pixel()));
}
