#pragma once

#include "terra_config.hpp"

#include <vector>

#ifdef USE_OPENCL
    #include "cl/cl2.hpp"
    #include "compute/engine_cl.hpp"
#endif

#include "types/bitmap.hpp"
#include "types/vec2.hpp"

namespace terra
{
    class uplift
    {
        std::vector<double> uplifts;

    private:
        const terra::bitmap* uplift_map;
        const std::vector<terra::vec2>* points;
        std::vector<double>* heights;

    public:
        uplift();
        uplift(const terra::bitmap& uplift_map,
               const std::vector<terra::vec2>& points,
               std::vector<double>& heights);

        void update();
#ifdef USE_OPENCL
        void update(terra::compute::engine_cl& engine);
#endif
    };
}