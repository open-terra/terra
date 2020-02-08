#include "terra/types/bitmap.hpp"

terra::bitmap::bitmap() :
    m_width(0), m_height(0), m_bits_per_sample(0), m_samples_per_pixel(0),
    m_num_pixels(0), m_raster(0)
{
}

terra::bitmap::bitmap(size_t width,
                      size_t height,
                      size_t bits_per_sample,
                      size_t samples_per_pixel,
                      size_t num_pixels,
                      terra::dynarray<uint8_t>& raster) : 
    m_width(width), m_height(height), m_bits_per_sample(bits_per_sample),
    m_samples_per_pixel(samples_per_pixel), m_num_pixels(num_pixels),
    m_raster(std::move(raster))
{
}


