#pragma once

#include <array>
#include <cstdint>
#include <memory>

namespace terra::noise
{
    class open_simplex
    {
    private:
        const double STRETCH_CONSTANT_2D =
            -0.211324865405187; // (1 / sqrt(2 + 1) - 1 ) / 2;
        const double SQUISH_CONSTANT_2D =
            0.366025403784439; // (sqrt(2 + 1) -1) / 2;
        const double STRETCH_CONSTANT_3D =
            -1.0 / 6.0; // (1 / sqrt(3 + 1) - 1) / 3;
        const double SQUISH_CONSTANT_3D = 1.0 / 3.0; // (sqrt(3+1)-1)/3;
        const double STRETCH_CONSTANT_4D =
            -0.138196601125011; // (1 / sqrt(4 + 1) - 1) / 4;
        const double SQUISH_CONSTANT_4D =
            0.309016994374947; // (sqrt(4 + 1) - 1) / 4;

        const double NORM_CONSTANT_2D = 47.0;
        const double NORM_CONSTANT_3D = 103.0;
        const double NORM_CONSTANT_4D = 30.0;

        /*
         * Gradients for 2D. They approximate the directions to the
         * vertices of an octagon from the center.
         */
        static constexpr const int8_t gradients2D[] = {
            5,
            2,
            2,
            5,
            -5,
            2,
            -2,
            5,
            5,
            -2,
            2,
            -5,
            -5,
            -2,
            -2,
            -5,
        };

        /*
         * Gradients for 3D. They approximate the directions to the
         * vertices of a rhombicuboctahedron from the center, skewed so
         * that the triangular and square facets can be inscribed inside
         * circles of the same radius.
         */
        static constexpr const int8_t gradients3D[] = {
            -11, 4,  4,   -4, 11, 4,   -4,  4,   11,  11, 4,   4,
            4,   11, 4,   4,  4,  11,  -11, -4,  4,   -4, -11, 4,
            -4,  -4, 11,  11, -4, 4,   4,   -11, 4,   4,  -4,  11,
            -11, 4,  -4,  -4, 11, -4,  -4,  4,   -11, 11, 4,   -4,
            4,   11, -4,  4,  4,  -11, -11, -4,  -4,  -4, -11, -4,
            -4,  -4, -11, 11, -4, -4,  4,   -11, -4,  4,  -4,  -11,
        };

        /*
         * Gradients for 4D. They approximate the directions to the
         * vertices of a disprismatotesseractihexadecachoron from the center,
         * skewed so that the tetrahedral and cubic facets can be inscribed
         * inside spheres of the same radius.
         */
        static constexpr const int8_t gradients4D[] = {
            3,  1,  1,  1,  1,  3,  1,  1,  1,  1,  3,  1,  1,  1,  1,  3,
            -3, 1,  1,  1,  -1, 3,  1,  1,  -1, 1,  3,  1,  -1, 1,  1,  3,
            3,  -1, 1,  1,  1,  -3, 1,  1,  1,  -1, 3,  1,  1,  -1, 1,  3,
            -3, -1, 1,  1,  -1, -3, 1,  1,  -1, -1, 3,  1,  -1, -1, 1,  3,
            3,  1,  -1, 1,  1,  3,  -1, 1,  1,  1,  -3, 1,  1,  1,  -1, 3,
            -3, 1,  -1, 1,  -1, 3,  -1, 1,  -1, 1,  -3, 1,  -1, 1,  -1, 3,
            3,  -1, -1, 1,  1,  -3, -1, 1,  1,  -1, -3, 1,  1,  -1, -1, 3,
            -3, -1, -1, 1,  -1, -3, -1, 1,  -1, -1, -3, 1,  -1, -1, -1, 3,
            3,  1,  1,  -1, 1,  3,  1,  -1, 1,  1,  3,  -1, 1,  1,  1,  -3,
            -3, 1,  1,  -1, -1, 3,  1,  -1, -1, 1,  3,  -1, -1, 1,  1,  -3,
            3,  -1, 1,  -1, 1,  -3, 1,  -1, 1,  -1, 3,  -1, 1,  -1, 1,  -3,
            -3, -1, 1,  -1, -1, -3, 1,  -1, -1, -1, 3,  -1, -1, -1, 1,  -3,
            3,  1,  -1, -1, 1,  3,  -1, -1, 1,  1,  -3, -1, 1,  1,  -1, -3,
            -3, 1,  -1, -1, -1, 3,  -1, -1, -1, 1,  -3, -1, -1, 1,  -1, -3,
            3,  -1, -1, -1, 1,  -3, -1, -1, 1,  -1, -3, -1, 1,  -1, -1, -3,
            -3, -1, -1, -1, -1, -3, -1, -1, -1, -1, -3, -1, -1, -1, -1, -3,
        };

        std::array<int16_t, 256> perm;
        std::array<int16_t, 256> permGradIndex3D;

    public:
        constexpr open_simplex(int64_t seed);

        constexpr double noise(double x, double y);
        constexpr double noise(double x, double y, double z);
        constexpr double noise(double x, double y, double z, double w);

    private:
        constexpr double extrapolate2(int xsb, int ysb, double dx, double dy);
        constexpr double extrapolate3(int xsb,
                                      int ysb,
                                      int zsb,
                                      double dx,
                                      double dy,
                                      double dz);
        constexpr double extrapolate4(int xsb,
                                      int ysb,
                                      int zsb,
                                      int wsb,
                                      double dx,
                                      double dy,
                                      double dz,
                                      double dw);
    };
} // namespace terra::noise
