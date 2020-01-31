add_executable(terra_tests
    test/dynarray.cpp
    test/math.cpp
    test/poisson_disc_sampling.cpp
    test/triangulation.cpp
    test/voronoi.cpp
    test/main.cpp
)

target_link_libraries(terra_tests 
    Terra 
    Catch2::Catch2
)

include(CTest)
include(Catch)
catch_discover_tests(terra_tests)