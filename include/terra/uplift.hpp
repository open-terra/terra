#pragma once

#include "terra_config.hpp"

#include <vector>

#ifdef TERRA_USE_OPENCL
    #include "cl/cl2.hpp"
    #include "compute/engine_cl.hpp"
#endif

#include "base_types.hpp"
#include "types/bitmap.hpp"
#include "types/dynarray.hpp"
#include "types/vec2.hpp"

namespace terra
{
    struct bitmap_uplift
    {
        size_t width;
        terra::dynarray<uint8_t> raster;

        explicit bitmap_uplift(const terra::bitmap& bitmap);

        tfloat at(const terra::vec2& p) const;
    };

    struct noise_uplift
    {
        size_t width, height;
        terra::vec2 scale;
        terra::dynarray<tfloat> noise_map;

        template<typename T>
        noise_uplift(const T& noise,
                     size_t terrain_width,
                     size_t terrain_height,
                     size_t noise_width,
                     size_t noise_height,
                     tfloat noise_scale);

        tfloat at(const terra::vec2& p) const;
    };

    struct linear_uplift
    {
        enum struct orient : int8_t
        {
            up = 0,
            down,
            left,
            right
        };

        orient orientation;
        size_t width, height;
        tfloat min, diff;

        linear_uplift(size_t width,
                      size_t height,
                      tfloat min,
                      tfloat max,
                      orient orientation = orient::left);

        tfloat at(const terra::vec2& p) const;
    };

    class uplift
    {
    public:
        tfloat factor;
        terra::dynarray<tfloat> uplifts;

    private:
        const std::vector<terra::vec2>* points;
        terra::dynarray<tfloat>* heights;

    public:
        uplift();
        template<typename UpliftFunc>
        uplift(const UpliftFunc& uplift_func,
               const std::vector<terra::vec2>& points,
               terra::dynarray<tfloat>& heights,
               tfloat factor);

        void update();
#ifdef TERRA_USE_OPENCL
        void update(terra::compute::engine_cl& engine);
#endif
    };
}

template<typename T>
terra::noise_uplift::noise_uplift(const T& noise,
                                  size_t terrain_width,
                                  size_t terrain_height,
                                  size_t noise_width,
                                  size_t noise_height,
                                  tfloat noise_scale) :
    width(noise_width), height(noise_height), scale(), noise_map(0)
{
    this->scale =
    {
        static_cast<tfloat>(noise_width) / terrain_width,
        static_cast<tfloat>(noise_height) / terrain_height
    };
    this->noise_map =
        noise.noise(0, 0, 0, noise_width, noise_height, 1, noise_scale);
}

template<typename UpliftFunc>
terra::uplift::uplift(const UpliftFunc& uplift_func,
                      const std::vector<terra::vec2>& points,
                      terra::dynarray<tfloat>& heights,
                      tfloat factor) :
    factor(factor), uplifts(points.size()), points(&points), heights(&heights)
{
    for (size_t i = 0; i < points.size(); ++i)
    {
        uplifts[i] = uplift_func.at(points[i]);
    }
}
