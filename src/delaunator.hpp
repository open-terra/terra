#pragma once

#include <algorithm>
#include <cmath>
#include <exception>
#include <iostream>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

#include "utils/fast_mod.hpp"

namespace delaunator
{
    constexpr double EPSILON = std::numeric_limits<double>::epsilon();
    constexpr size_t INVALID_INDEX = std::numeric_limits<size_t>::max();

    struct DelaunatorPoint
    {
        size_t i;
        double x;
        double y;
        size_t t;
        size_t prev;
        size_t next;
        bool removed;
    };

    class Delaunator 
    {
    public:
        std::vector<double> const& coords;
        std::vector<size_t> triangles;
        std::vector<size_t> halfedges;
        std::vector<size_t> hull_prev;
        std::vector<size_t> hull_next;
        std::vector<size_t> hull_tri;
        size_t hull_start;

        Delaunator(std::vector<double> const& in_coords);

        double get_hull_area();

    private:
        std::vector<size_t> m_hash;
        double m_center_x;
        double m_center_y;
        size_t m_hash_size;
        std::vector<size_t> m_edge_stack;

        size_t legalize(size_t a);
        size_t hash_key(double x, double y) const;
        size_t add_triangle(size_t i0, size_t i1, size_t i2, size_t a, size_t b, size_t c);
        void link(size_t a, size_t b);
    };
}
