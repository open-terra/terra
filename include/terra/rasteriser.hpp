#pragma once

#include <limits>
#include <span>
#include <vector>

#include "concepts.hpp"
#include "concepts_helpers.hpp"
#include "math/ceil.hpp"
#include "math/floor.hpp"
#include "types/dynarray.hpp"
#include "types/hash_grid.hpp"
#include "types/vec2.hpp"

namespace terra
{
    class rasteriser
    {
    public:
        template<class Array> requires terra::Container<Array, tfloat>
        inline rasteriser(const Array& heights,
                          const terra::hash_grid& hash_grid);

        template<class T>
        inline terra::dynarray<T> raster(size_t width, size_t height);

    private:
        terra::vec2 m_scale;
        const std::span<const tfloat> m_heights;
        const terra::hash_grid* m_hash_grid;

        inline tfloat get_height_at(size_t x, size_t y) const;
    };
}

template<class Array> requires terra::Container<Array, tfloat>
terra::rasteriser::rasteriser(const Array& heights,
                              const terra::hash_grid& hash_grid) :
    m_scale(0.0), m_heights(terra::to_span<tfloat>(heights)), m_hash_grid(&hash_grid)
{
}


template<>
inline terra::dynarray<tfloat> terra::rasteriser::raster<tfloat>(size_t width, size_t height)
{
    this->m_scale = terra::vec2(this->m_hash_grid->get_grid_width() / width,
                                this->m_hash_grid->get_grid_height() / height);

    terra::dynarray<tfloat> raster(width * height);
    for (size_t y = 0; y < height; ++y)
    {
        size_t y_offset = y * width;
        for (size_t x = 0; x < width; ++x)
        {
            raster[y_offset + x] = this->get_height_at(x, y);
        }
    }

    return raster;
}

template<class T>
terra::dynarray<T> terra::rasteriser::raster(size_t width, size_t height)
{
    this->m_scale = terra::vec2(this->m_hash_grid->get_grid_width() / width,
                                this->m_hash_grid->get_grid_height() / height);

    constexpr T t_min = std::numeric_limits<T>::min();
    constexpr T t_max = std::numeric_limits<T>::max();
    T t_diff = t_max - t_min;

    auto in_min = this->m_heights[0];
    auto in_max = this->m_heights[0];
    for (size_t i = 1; i < this->m_heights.size(); ++i)
    {
        const auto h = this->m_heights[i];
        if (h < in_min)
        {
            in_min = h;
        }
        else if (h > in_max)
        {
            in_max = h;
        }
    }
    auto in_diff = in_max - in_min;

    terra::dynarray<T> raster(width * height);
    for (size_t y = 0; y < height; ++y)
    {
        size_t y_offset = y * width;
        for (size_t x = 0; x < width; ++x)
        {
            const tfloat height = this->get_height_at(x, y);
            const tfloat norm_height = (height - in_min) / in_diff;
            const T out_height = (norm_height * t_diff) + t_min;
            raster[y_offset + x] = out_height;
        }
    }

    return raster;
}

tfloat terra::rasteriser::get_height_at(size_t x, size_t y) const
{
    const size_t x_offset = terra::math::floor<size_t>(x * this->m_scale.x);
    const size_t y_offset = terra::math::floor<size_t>(y * this->m_scale.y);
    const size_t width =
        std::min(terra::math::ceil<size_t>((x + 1) * this->m_scale.x),
                 this->m_hash_grid->get_grid_width());
    const size_t height =
        std::min(terra::math::ceil<size_t>((y + 1) * this->m_scale.y),
                 this->m_hash_grid->get_grid_height());

    auto cells = this->m_hash_grid->get_cells(x_offset, y_offset, width, height);
    tfloat h = 0.0;
    for (auto cell : cells)
    {
        h += this->m_heights[cell];
    }

    return h / cells.size();
}
