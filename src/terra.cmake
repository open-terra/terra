add_library(terra STATIC
    src/noise/open_simplex.cpp
    src/noise/summed_noise.cpp
    src/types/hash_grid.cpp
    src/delaunator.cpp
    src/poisson_disc_sampler.cpp
    src/voronoi.cpp
)