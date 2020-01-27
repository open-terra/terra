#pragma once

#include <vector>

#include "types/vec2.hpp"

namespace terra
{
    class uplift
    {
    private:
        const std::vector<terra::vec2>* points;
        const std::vector<double>* heights;

    public:
        uplift();
        uplift(const std::vector<terra::vec2>& points,
               const std::vector<double>& heigts);

        void update();
    };
}