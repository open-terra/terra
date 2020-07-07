#pragma once

#include <cstdint>
#include <random>
#include <stack>
#include <vector>

#include "base_types.hpp"
#include "types/hash_grid.hpp"
#include "types/vec2.hpp"

namespace terra
{
    // This implements the algorithm described in 'Fast Poisson Disk Sampling in
    // Arbitrary Dimensions' by Robert Bridson. This produces a random set of
    // points such that no two points are closer than min_distance apart or
    // further apart than twice that distance.
    //
    // Parameters
    //
    //   conf    - The configuration, as detailed above.
    //
    //   random  - A callback of the form tfloat(tfloat limit) that returns a
    //   random
    //             value ranging from 0 (inclusive) to limit (exclusive).
    //
    //   in_area - A callback of the form bool(terra:vec2) that returns whether
    //   a terra:vec2
    //             is within a valid area. This can be used to create shapes
    //             other than rectangles. terra:vec2s can't be outside of the
    //             defined limits of the width and height specified. See the
    //             notes section for more.
    //
    //   output  - A callback of the form void(terra:vec2). All terra:vec2s that
    //   are part of
    //             the final Poisson disc distribution are passed here.
    //
    // Notes
    //
    //   The time complexity is O(n) where n is the number of terra:vec2s.
    //
    //   The in_area callback must prevent terra:vec2s from leaving the region
    //   defined by width and height (i.e., 0 <= x < width and 0 <= y < height).
    //   If this is not done invalid memory accesses will occur and most likely
    //   a segmentation fault.
    class poisson_disc_sampler
    {
    public:
        //   width, height - Defines the range of x as (0, width] and the range
        //                   of y as (0, height].
        tfloat width;
        tfloat height;

        // min_distance - The smallest distance allowed between two
        //                terra:vec2s.
        //                Also, terra:vec2s will never be further apart than
        //                twice this distance.
        tfloat min_distance;

        // max_attempts - The algorithm stochastically attempts to place a new
        //                terra:vec2 around a current terra:vec2.
        //                This number limits the number of attempts per
        //                terra:vec2.
        //                A lower number will speed up the algorithm but at some
        //                cost, possibly significant, to the result's
        //                aesthetics.
        // size_t max_attempts = 30;

        // start - An optional parameter. If set to anything other than
        //         terra:vec2's default values (infinity, infinity) the
        //         algorithm will start from this terra:vec2.
        //         Otherwise a terra:vec2 is chosen randomly.
        //         Expected to be within the region defined by width and height.
        // terra::vec2 start = {infinity, infinity};
    private:
        std::default_random_engine engine;
        std::uniform_real_distribution<tfloat> distribution;
        std::vector<terra::vec2> points;
        std::stack<size_t> active;
        terra::hash_grid* hash_grid;

    public:
        constexpr static tfloat infinity =
            std::numeric_limits<tfloat>::infinity();

        poisson_disc_sampler();
        ~poisson_disc_sampler();

        std::vector<terra::vec2> sample(size_t width,
                                        size_t height,
                                        tfloat min_distance,
                                        size_t max_attempts = 30,
                                        terra::hash_grid** hash_grid = nullptr,
                                        terra::vec2 start = {infinity,
                                                             infinity});

    private:
        tfloat random(tfloat range);
        terra::vec2 point_around(terra::vec2 p);
        bool in_area(const terra::vec2& p);
        void add(const terra::vec2& p);
        bool point_too_close(const terra::vec2& p);
    };
} // namespace terra
