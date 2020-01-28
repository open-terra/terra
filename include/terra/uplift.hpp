#pragma once

#include <vector>

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
    };
}