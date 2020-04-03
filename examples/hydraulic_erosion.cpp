#include <chrono>

#include "terra/terra.hpp"

int main(int argc, char** argv)
{
    size_t width = 512, height = 512;
    size_t offset_x = 842, offset_y = 521, offset_z = 1341;
    tfloat radius = 2.0;

    std::vector<terra::vec2> points;
    std::unique_ptr<terra::hash_grid> hash_grid;
    {
        terra::hash_grid* temp_grid = nullptr;
        auto sampler = terra::poisson_disc_sampler();
        points = sampler.sample(width, height, radius, 100, &temp_grid);

        hash_grid = std::unique_ptr<terra::hash_grid>(temp_grid);
    }
    const size_t node_count = points.size();

    terra::dynarray<terra::triangle> tris(0);
    {
        terra::delaunator d;
        d.triangulate(points);
        tris = terra::dynarray<terra::triangle>(d.triangles.size() / 3);

        for (size_t i = 0; i < tris.size(); ++i)
        {
            size_t index = i * 3;
            const size_t p0 = index;
            const size_t p1 = index + 1;
            const size_t p2 = index + 2;

            const size_t v0 = d.triangles[p0];
            const size_t v1 = d.triangles[p1];
            const size_t v2 = d.triangles[p2];

            tris[i] = terra::triangle(v0, v1, v2);
        }
    }

    terra::dynarray<tfloat> heights(node_count);
    {
        terra::noise::ridged_noise noise(1337);
        auto res = noise.noise(offset_x, offset_y, offset_z, width, height, 1, 0.5);
        for (size_t i = 0; i < res.size(); ++i)
        {
            res[i] *= 20.0;
        }

        for (size_t i = 0; i < node_count; ++i)
        {
            const auto& p = points[i];
            size_t x = terra::math::floor<size_t>(p.x);
            size_t y = terra::math::floor<size_t>(p.y);
            heights[i] = res[(y * width) + x];
        }
    }

    terra::dynarray<terra::vec3> verts(node_count);
    for (size_t i = 0; i < node_count; ++i)
    {
        verts[i] = terra::make_vec3(points[i], heights[i]);
    }
    terra::io::obj::write_obj("fbm_terrain.obj", verts, tris);

    terra::erosion::hydraulic_graph erosion(points, *hash_grid.get(), tris, heights, 1337);
    // erosion.seed =
    // std::chrono::high_resolution_clock::now().time_since_epoch().count();
    erosion.erode(100000, terra::rect<tfloat>(0, 0, 512, 512));

    // recreate the vertex buffer with the updated height information
    for (size_t i = 0; i < node_count; ++i)
    {
        verts[i] = terra::make_vec3(points[i], heights[i]);
    }
    terra::io::obj::write_obj("he_terrain.obj", verts, tris);

    return 0;
}
