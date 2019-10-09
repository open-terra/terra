#pragma once

#include <cmath>

namespace cp
{
    struct Vector3
    {
        double x, y, z;

        Vector3()
        {
        }

        Vector3(double value)
        {
            this->x = value;
            this->y = value;
            this->z = value;
        }

        Vector3(double x, double y)
        {
            this->x = x;
            this->y = y;
            this->z = 0.0;
        }

        Vector3(double x, double y, double z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }

        static inline double DistanceSquared(Vector3 &l, Vector3 &r)
        {
            double dx = l.x - r.x;
            double dy = l.y - r.y;
            double dz = l.z - r.z;
            return (dx * dx) + (dy * dy) + (dz * dz);
        }

        static inline double Distance(Vector3 &l, Vector3 &r)
        {
            double dx = l.x - r.x;
            double dy = l.y - r.y;
            double dz = l.z - r.z;
            return std::sqrt((dx * dx) + (dy * dy) + (dz * dz));
        }
    };
}