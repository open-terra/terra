#pragma once

#include <cmath>

namespace Terra
{
    struct Vector2
    {
        double x, y;

        Vector2()
        {
			this->x = 0.0;
			this->y = 0.0;
        }

        Vector2(double value)
        {
            this->x = value;
            this->y = value;
        }

        Vector2(double x, double y)
        {
            this->x = x;
            this->y = y;
        }

        static inline double DistanceSquared(Vector2 &l, Vector2 &r)
        {
            double dx = l.x - r.x;
            double dy = l.y - r.y;
            return (dx * dx) + (dy * dy);
        }

        static inline double Distance(Vector2 &l, Vector2 &r)
        {
            double dx = l.x - r.x;
            double dy = l.y - r.y;
            return std::sqrt((dx * dx) + (dy * dy));
        }
    };
}