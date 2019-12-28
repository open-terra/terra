cmake_minimum_required(VERSION 3.10)

# Project name
project(Terra
    VERSION 0.1.0
    DESCRIPTION ""
    LANGUAGES C CXX
)

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

# Do stuff depending on the compiler
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "-W -Wall -Wextra -Wpedantic -Wunused-value -Wold-style-cast -std=c++17")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "/W4 /std:c++17")
    set(CMAKE_CXX_FLAGS_DEBUG "/O0 /ZI")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob2")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include(src/terra.cmake)

if(BUILD_TESTING)
    include(test/tests.cmake)
endif()

INSTALL(TARGETS terra LIBRARY DESTINATION lib)
INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)
