#include "terra/delaunay.hpp"

#include <tuple>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "terra/types/dynarray.hpp"

terra::delaunay::delaunay() : triangles()
{
}

void terra::delaunay::triangulate(const std::vector<terra::vec2>& points,
                                  const terra::hash_grid& hash_grid)
{
    const size_t num_points = points.size();
    const size_t max_triangles = num_points < 3 ? 1 : 2 * num_points - 5;

    // create internal buffers
    this->vertices.resize(points.size());
    this->edges.reserve(max_triangles);
    this->triangles.reserve(max_triangles);
    this->inserted_verts.resize(points.size());

    // fill vertices
    for (size_t i = 0; i < num_points; ++i)
    {
        this->vertices[i].v = &this->points->at(i);
    }

    this->add_triangle(0, 1, 2);
    for (size_t i = 0; i < 3; ++i)
    {
        inserted_verts[i] = true;
    }

    for (size_t i = 3; i < num_points; ++i)
    {
        std::vector<size_t> inserted;
        std::vector<size_t> uninserted;
        std::tie(inserted, uninserted) = this->split_inserted(
            this->hash_grid->get_neighbours(this->points->at(i)));

        for (const auto n : uninserted)
        {
            this->insert(n);
            inserted.push_back(n);
        }
    }
}

void terra::delaunay::insert(size_t i, const std::vector<size_t>& neighbours)
{


    this->inserted_verts[i] = true;
}

terra::delaunay::triangle& terra::delaunay::add_triangle(size_t v0,
                                                         size_t v1,
                                                         size_t v2)
{
    triangle tri;
    tri.v0 = &this->vertices.at(v0);
    tri.v1 = &this->vertices.at(v1);
    tri.v2 = &this->vertices.at(v2);
    tri.e0 = &this->add_edge(v0, v1);
    tri.e1 = &this->add_edge(v1, v2);
    tri.e2 = &this->add_edge(v2, v0);
    tri.circum_centre = this->circum_centre(tri);

    this->triangles.push_back(tri);

    // add triangle to vertex tri lists
    tri.v0->tris.push_back(&this->triangles.back());
    tri.v1->tris.push_back(&this->triangles.back());
    tri.v2->tris.push_back(&this->triangles.back());

    return this->triangles.back();
}

terra::delaunay::edge& terra::delaunay::add_edge(size_t v0, size_t v1)
{
    edge edge;
    edge.v0 = &this->vertices.at(v0);
    edge.v1 = &this->vertices.at(v1);

    this->edges.push_back(edge);

    return this->edges.back();
}

std::pair<std::vector<size_t>, std::vector<size_t>>
terra::delaunay::split_inserted(std::vector<size_t> connected)
{
    std::vector<size_t> inserted;
    std::vector<size_t> uninserted;

    for (const auto n : connected)
    {
        if (this->inserted_verts[n])
        {
            inserted.push_back(n);
        }
        else
        {
            uninserted.push_back(n);
        }
    }

    return std::make_pair(inserted, uninserted);
}

terra::delaunay::circle terra::delaunay::circum_centre(
    const terra::delaunay::triangle& tri)
{
    // fetch vertex points
    const auto& p0 = *tri.v0->v;
    const auto& p1 = *tri.v1->v;
    const auto& p2 = *tri.v2->v;

    // calculate the centre of the circle
    const tfloat dx = p0.x + p1.x + p2.x;
    const tfloat dy = p0.y + p1.y + p2.y;
    const terra::vec2 centre = { dx / 3.0f, dy / 3.0f };

    // get the squared radius of the circle
    const tfloat r2 = glm::distance2(centre, p0);

    return { centre, r2 };
}