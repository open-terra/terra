#pragma once

#include <stdexcept>

#include "../utils/fast_sqrt.hpp"

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
            return Utils::FastSqrt((dx * dx) + (dy * dy));
        }

        static constexpr double Norm(const Terra::Vector2& p)
        {
            return (p.x * p.x) + (p.y * p.y);
        }

        static constexpr double VectorProduct(const Terra::Vector2 &l, const Terra::Vector2 &r)
        {
            return (l.x * r.y) - (l.y * r.x);
        }
 
        static constexpr double ScalarProduct(const Terra::Vector2 &l, const Terra::Vector2 &r)
        {
            return (l.x * r.x) + (l.y * r.y);
        }

        static constexpr int32_t Compare(const Terra::Vector2& l, const Terra::Vector2& r)
        {
            return (l.y != r.y) ? (l.y < r.y ? -1 : 1) : (l.x < r.x ? -1 : 1);
        }

        constexpr Vector2 operator+(const Vector2 &p) const
        {
            return Vector2(this->x + p.x, this->y + p.y);
        }

        constexpr Vector2 operator-(const Vector2 &p) const
        {
            return Vector2(this->x - p.x, this->y - p.y);
        }

        constexpr Vector2 operator*(const double t) const
        {
            return Vector2(this->x * t, this->y * t);
        }
    };

    constexpr bool operator==(const Vector2& lhs, const Vector2& rhs)
    {
        return (lhs.x == rhs.x) && (lhs.y == rhs.y);
    }
    constexpr bool operator!=(const Vector2& lhs, const Vector2& rhs) { return !(lhs == rhs); }

    constexpr bool operator< (const Vector2& lhs, const Vector2& rhs)
    {
        return (lhs.y == rhs.y) ? (lhs.x < rhs.x) : lhs.y < rhs.y;
    }
    constexpr bool operator> (const Vector2& lhs, const Vector2& rhs) { return rhs < lhs; }
    constexpr bool operator<=(const Vector2& lhs, const Vector2& rhs) { return !(lhs > rhs); }
    constexpr bool operator>=(const Vector2& lhs, const Vector2& rhs) { return !(lhs < rhs); }

    constexpr void swap(Terra::Vector2& l, Terra::Vector2& r)
    {
        auto c = l;
        l = r;
        r = c;
    }
}
