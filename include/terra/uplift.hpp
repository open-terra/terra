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
    class uplift
    {
        terra::dynarray<tfloat> uplifts;

    private:
        const terra::bitmap* uplift_map;
        const std::vector<terra::vec2>* points;
        terra::dynarray<tfloat>* heights;

    public:
        uplift();
        uplift(const terra::bitmap& uplift_map,
               const std::vector<terra::vec2>& points,
               terra::dynarray<tfloat>& heights);

        void update();
#ifdef TERRA_USE_OPENCL
        void update(terra::compute::engine_cl& engine);
#endif
    };
}