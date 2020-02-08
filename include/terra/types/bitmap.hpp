#pragma once

#include <cstdint>

#include "dynarray.hpp"

namespace terra
{
    struct argb_t
    {
    public:
        uint8_t alpha, red, green, blue;
    };

    struct bitmap
    {
    public:
        enum struct channel : size_t
        {
            alpha = 0,
            red,
            green,
            blue
        };

    private:
        size_t m_width;
        size_t m_height;
        size_t m_bits_per_sample;
        size_t m_samples_per_pixel;
        size_t m_num_pixels;
        terra::dynarray<uint8_t> m_raster;

    public:
        bitmap();
        bitmap(size_t width,
               size_t height,
               size_t bits_per_sample,
               size_t samples_per_pixel,
               size_t num_pixels,
               terra::dynarray<uint8_t>& raster);

        inline size_t width() const;
        inline size_t height() const;
        inline size_t bits_per_sample() const;
        inline size_t samples_per_pixel() const;
        inline size_t size() const;

        template<typename T = uint8_t>
        inline const T* get() const;
        template<typename T = uint8_t>
        terra::dynarray<T> get_channel(channel c) const;
    };
}

inline size_t terra::bitmap::width() const
{
    return this->m_width;
}
inline size_t terra::bitmap::height() const
{
    return this->m_height;
}
inline size_t terra::bitmap::bits_per_sample() const
{
    return this->m_bits_per_sample;
}
inline size_t terra::bitmap::samples_per_pixel() const
{
    return this->m_samples_per_pixel;
}
inline size_t terra::bitmap::size() const
{
    return this->m_num_pixels;
}

template<typename T>
inline const T* terra::bitmap::get() const
{
    return reinterpret_cast<const T*>(this->m_raster.data());
}

template<typename T>
terra::dynarray<T> terra::bitmap::get_channel(bitmap::channel c) const
{
    terra::dynarray<T> data(this->num_pixels);

    T* raster_view = this->get<T>();

    size_t sample = static_cast<size_t>(c);
    for (size_t i = 0; i < this->m_num_pixels; ++i)
    {
        data[i] = raster_view[(i * this->m_samples_per_pixel) + sample];
    }

    return data;
}
