#pragma once

#include <cmath>
#include <stdexcept>

namespace Terra
{
    struct Vector2
    {
        double x, y;

        constexpr Vector2() : x(0.0), y(0.0) {}

        constexpr Vector2(double value) : x(value), y(value) {}

        constexpr Vector2(double x, double y) : x(x), y(y) {}

        constexpr Vector2(std::initializer_list<double> values) : x(0), y(0)
        {
            if (values.size() != 2)
            {
                throw std::invalid_argument("initializer list should only contain 2 values");
            }

            const double* ptr = values.begin();
            this->x = *ptr;
            this->y = *(ptr + 1);
        }

        constexpr Vector2& operator=(const Vector2& other)
        {
            this->x = other.x;
            this->y = other.y;

            return *this;
        }

        static constexpr double DistanceSquared(const Vector2 &l, const Vector2 &r)
        {
            double dx = l.x - r.x;
            double dy = l.y - r.y;
            return (dx * dx) + (dy * dy);
        }

        static constexpr double Distance(const Vector2 &l, const Vector2 &r)
        {
            double dx = l.x - r.x;
            double dy = l.y - r.y;
            return std::sqrt((dx * dx) + (dy * dy));
        }
    };

    constexpr void swap(Terra::Vector2& a, Terra::Vector2& b)
    {
        auto c = a;
        a = b;
        b = c;
    }
}
