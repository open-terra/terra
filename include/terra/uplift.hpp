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
        size_t width;
        terra::dynarray<tfloat> noise_map;

        template<typename T>
        noise_uplift(const T& noise,
                              size_t width,
                              size_t height,
                              tfloat scale);

        tfloat at(const terra::vec2& p) const;
    };

    class uplift
    {
        terra::dynarray<tfloat> uplifts;

    private:
        const std::vector<terra::vec2>* points;
        terra::dynarray<tfloat>* heights;

    public:
        uplift();
        template<typename UpliftFunc>
        uplift(const UpliftFunc& uplift_func,
               const std::vector<terra::vec2>& points,
               terra::dynarray<tfloat>& heights);

        void update();
#ifdef TERRA_USE_OPENCL
        void update(terra::compute::engine_cl& engine);
#endif
    };
}

template<typename T>
terra::noise_uplift::noise_uplift(const T& noise,
                                  size_t width,
                                  size_t height,
                                  tfloat scale) : width(width), noise_map(0)
{
    this->noise_map = noise.noise(0, 0, 0, width, height, 1, scale);
}

template<typename UpliftFunc>
terra::uplift::uplift(const UpliftFunc& uplift_func,
                      const std::vector<terra::vec2>& points,
                      terra::dynarray<tfloat>& heights) :
    uplifts(points.size()), points(&points), heights(&heights)
{
    for (size_t i = 0; i < points.size(); ++i)
    {
        uplifts[i] = uplift_func.at(points[i]);
    }
}
