#pragma once

#include <cmath>

namespace cp
{
    struct Vector3
    {
        double x, y, z;

        constexpr Vector3() : x(0.0), y(0.0), z(0.0) {}
        constexpr Vector3(double value) : x(value), y(value), z(value) {}
        constexpr Vector3(double x, double y) : x(x), y(y), z(0.0) {}
        constexpr Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

        static constexpr double DistanceSquared(Vector3 &l, Vector3 &r)
        {
            double dx = l.x - r.x;
            double dy = l.y - r.y;
            double dz = l.z - r.z;
            return (dx * dx) + (dy * dy) + (dz * dz);
        }

        static constexpr double Distance(Vector3 &l, Vector3 &r)
        {
            double dx = l.x - r.x;
            double dy = l.y - r.y;
            double dz = l.z - r.z;
            return std::sqrt((dx * dx) + (dy * dy) + (dz * dz));
        }
    };
}