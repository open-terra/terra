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

        Vector2& operator=(const Vector2& other)
        {
            this->x = other.x;
            this->y = other.y;

            return *this;
        }

        static inline double DistanceSquared(const Vector2 &l, const Vector2 &r)
        {
            double dx = l.x - r.x;
            double dy = l.y - r.y;
            return (dx * dx) + (dy * dy);
        }

        static inline double Distance(const Vector2 &l, const Vector2 &r)
        {
            double dx = l.x - r.x;
            double dy = l.y - r.y;
            return std::sqrt((dx * dx) + (dy * dy));
        }
    };

    void swap(Terra::Vector2& a, Terra::Vector2& b)
    {
        auto c = a;
        a = b;
        b = c;
    }
}
