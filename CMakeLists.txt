cmake_minimum_required(VERSION 3.10)

# detect if Terra is being bundled,
# disable testsuite in that case
if(NOT DEFINED PROJECT_NAME)
  set(NOT_SUBPROJECT ON)
endif()

# Project name
project(Terra
    VERSION 0.1.0
    DESCRIPTION ""
    LANGUAGES C CXX
)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if(BUILD_EXAMPLES)
    # benchmark needed for testing
    find_package(benchmark CONFIG REQUIRED)
endif()

if(BUILD_TESTING)
    # Catch2 needed for testing
    find_package(Catch2 CONFIG REQUIRED)
endif()

# GLM used for vecN types
find_package(glm CONFIG REQUIRED)

# FastNoiseSIMD for 3d noise functions
find_package(FastNoiseSIMD CONFIG REQUIRED)

# Boost Polygon for Voronoi

# Libtiff
find_path(LIBTIFF_INCLUDE_DIR tiff.h)
find_library(LIBTIFF_LIB tiff)
find_library(LIBTIFF_XX_LIB tiffxx)

option(TERRA_USE_DOUBLE "Whether to use double precision floating point values." OFF)

configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/cmake/terra_config.hpp.in
    ${CMAKE_CURRENT_LIST_DIR}/include/terra/terra_config.hpp
    @ONLY
)

# Do stuff depending on the compiler
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "-W -Wall -Wextra -Wpedantic -Wunused-value -Wold-style-cast -std=c++17")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "/W4 /EHsc /std:c++17")
    set(CMAKE_CXX_FLAGS_DEBUG "/O0 /ZI")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob2")
endif()

set(TERRA_SOURCES
    src/erosion/hydraulic_graph.cpp
    src/erosion/hydraulic_grid.cpp
    src/io/image.cpp
    src/io/tiff.cpp
    src/io/wavefront_obj.cpp
    src/noise/erosive_noise.cpp
    src/noise/sdnoise1234.cpp
    src/stb/stb_impl.cpp
    src/types/bitmap.cpp
    src/types/flow_graph.cpp
    src/types/hash_grid.cpp
    src/delaunator.cpp
    src/poisson_disc_sampler.cpp
    src/uplift.cpp
    src/voronoi.cpp
)

add_library(Terra STATIC
    ${TERRA_SOURCES}
)

set_target_properties(Terra
    PROPERTIES 
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CMAKE_DEBUG_POSTFIX "d"
        OUTPUT_NAME "terra"
)

target_include_directories(Terra PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE ${LIBTIFF_INCLUDE_DIR}
)

set(TERRA_LINK_LIBS
    glm
    FastNoiseSIMD
    ${LIBTIFF_LIB}
    ${LIBTIFF_XX_LIB}
)

target_link_libraries(Terra PUBLIC
    ${TERRA_LINK_LIBS}
)

if(BUILD_EXAMPLES)
    include(examples/examples.cmake)
endif()

if(BUILD_TESTING)
    include(test/tests.cmake)
endif()

# Only perform the installation steps when not being used as
# a subproject via `add_subdirectory`, or the destinations will break
if(NOT_SUBPROJECT)
    set(Terra_CMAKE_CONFIG_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Terra")

    configure_package_config_file(
        ${CMAKE_CURRENT_LIST_DIR}/cmake/TerraConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/TerraConfig.cmake
        INSTALL_DESTINATION ${Terra_CMAKE_CONFIG_DESTINATION}
    )

    # create and install an export set for Terra target as Terra
    install(
        TARGETS Terra EXPORT TerraTargets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )


    install(EXPORT TerraTargets DESTINATION ${Terra_CMAKE_CONFIG_DESTINATION})

    write_basic_package_version_file(
      "${CMAKE_CURRENT_BINARY_DIR}/TerraConfigVersion.cmake"
      COMPATIBILITY SameMajorVersion
    )

    install(TARGETS Terra LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")
    install(DIRECTORY "include/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

    install(
        FILES
            "${CMAKE_CURRENT_BINARY_DIR}/TerraConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/TerraConfigVersion.cmake"
        DESTINATION ${Terra_CMAKE_CONFIG_DESTINATION}
    )
endif()
