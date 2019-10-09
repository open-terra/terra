add_library(terra STATIC
    src/generator/grid/hash_grid.cpp
    src/generator/grid/poisson_disc_sampler.cpp
    src/generator/noise/open_simplex.cpp
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})