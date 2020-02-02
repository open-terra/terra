cmake_minimum_required(VERSION 3.10)

# detect if Catch is being bundled,
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

# Use C++17
set(CMAKE_CXX_STANDARD 17)

# Use C11
set(CMAKE_C_STANDARD 11)

if(BUILD_TESTING)
    # Catch2 needed for testing
    find_package(Catch2 CONFIG REQUIRED)
endif()

# GLM used for vecN types
find_package(glm CONFIG REQUIRED)

option(USE_OPENCL "" ON)
if(USE_OPENCL)
    find_package(OpenCL REQUIRED)
endif()

# Boost Polygon for Voronoi

# Libtiff
find_path(LIBTIFF_INCLUDE_DIR tiff.h)
find_library(LIBTIFF_LIB tiff)
find_library(LIBTIFF_XX_LIB tiffxx)

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
    src/io/tiff.cpp
    src/io/wavefront_obj.cpp
    src/noise/open_simplex.cpp
    src/noise/summed_noise.cpp
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

set_target_properties(Terra PROPERTIES OUTPUT_NAME "terra")

target_include_directories(Terra PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE ${LIBTIFF_INCLUDE_DIR}
)

target_link_libraries(Terra PUBLIC
    glm
    ${LIBTIFF_LIB}
    ${LIBTIFF_XX_LIB}
)

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

add_custom_command(
    TARGET Terra POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/kernels
            ${CMAKE_CURRENT_BINARY_DIR}/kernels
)
