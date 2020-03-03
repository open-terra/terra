#pragma once

#include <utility>
#include <vector>

#include "base_types.hpp"
#include "types/hash_grid.hpp"
#include "types/vec2.hpp"

namespace terra
{
    class delaunay
    {
    public:
        struct vertex;
        struct edge;
        struct triangle;

        struct circle
        {
            terra::vec2 centre;
            // radius squared
            tfloat r2;

            // returns whether the point lies within the circle
            bool point_within(const terra::vec2& p);
        };

        struct vertex
        {
            // internal vertex
            terra::vec2* v;
            // connected triangles
            std::vector<triangle*> tris;
        };

        struct edge
        {
            vertex* v0;
            vertex* v1;
            triangle* t0;
            triangle* t1;
        };

        struct triangle
        {
            vertex* v0;
            vertex* v1;
            vertex* v2;
            edge* e0;
            edge* e1;
            edge* e2;
            // bounding circle of the triangle
            circle circum_centre;

            // returns whether the point lies within the triangle
            bool point_within(const terra::vec2& p);
        };

        std::vector<vertex> vertices;
        std::vector<edge> edges;
        std::vector<triangle> triangles;
    private:
        std::vector<bool> inserted_verts;
        std::vector<terra::vec2>* points;
        terra::hash_grid* hash_grid;

    public:
        delaunay();

        void triangulate(const std::vector<terra::vec2>& points,
                         const terra::hash_grid& hash_grid);
    
    private:
        void insert(size_t i, const std::vector<size_t>& inserted);
        triangle& add_triangle(size_t v0, size_t v1, size_t v2);
        edge& add_edge(size_t v0, size_t v1);

        // splits a vector of vertex indexs by whether they've been inserted
        // into the triangulation yet
        std::pair<std::vector<size_t>, std::vector<size_t>> split_inserted(
            std::vector<size_t> connected);
        // gets the bounding circle of a triangle
        circle circum_centre(const triangle& triangle);
    };
} // namespace terra
