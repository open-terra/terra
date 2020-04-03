#pragma once

#include <chrono>

#include "../base_types.hpp"

namespace terra::erosion
{
    class hydraulic_config
    {
    public:
        size_t seed =   std::chrono::high_resolution_clock::now()
                            .time_since_epoch().count();
        tfloat erosion_radius = 3.0;
        tfloat inertia = .05f; // At zero, water will instantly change direction to flow downhill. At 1, water will never change direction. 
        tfloat sediment_capacity_factor = 4; // Multiplier for how much sediment a droplet can carry
        tfloat min_sediment_capacity = .01f; // Used to prevent carry capacity getting too close to zero on flatter terrain
        tfloat erode_speed = .3f;
        tfloat deposit_speed = .3f;
        tfloat evaporate_speed = .01f;
        tfloat gravity = 4;
        size_t max_droplet_lifetime = 30;

        tfloat initial_water_volume = 1;
        tfloat initial_speed = 1;
    };
}
