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
include(cmake/CPM.cmake)

CPMAddPackage(
  NAME Format.cmake
  VERSION 1.6
  GITHUB_REPOSITORY TheLartians/Format.cmake
  OPTIONS # enable cmake formatting (optional)
          "FORMAT_CHECK_CMAKE ON"
          # path to exclude (optional, supports regular expressions)
          "CMAKE_FORMAT_EXCLUDE cmake/CPM.cmake"
)

if(BUILD_EXAMPLES)
    # benchmark needed for testing
    find_package(benchmark CONFIG REQUIRED)
endif()

if(BUILD_TESTING)
    # Catch2 needed for testing
    find_package(Catch2 CONFIG REQUIRED)
endif()

option(TERRA_USE_GLM "Whether to use GLM." ON)

if(TERRA_USE_GLM)
    # GLM used for vecN types
    find_package(glm CONFIG REQUIRED)
endif()

configure_file(
    ${CMAKE_CURRENT_LIST_DIR}/cmake/terra_config.hpp.in
    ${CMAKE_CURRENT_LIST_DIR}/include/terra/terra_config.hpp
    @ONLY
)

add_library(Terra INTERFACE)

target_include_directories(Terra INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
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
    install(TARGETS Terra EXPORT TerraTargets DESTINATION ${CMAKE_INSTALL_LIBDIR})

    install(EXPORT TerraTargets DESTINATION ${Terra_CMAKE_CONFIG_DESTINATION})

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/TerraConfigVersion.cmake"
        COMPATIBILITY SameMajorVersion
    )

    install(DIRECTORY "include/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

    install(
        FILES
            "${CMAKE_CURRENT_BINARY_DIR}/TerraConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/TerraConfigVersion.cmake"
        DESTINATION ${Terra_CMAKE_CONFIG_DESTINATION}
    )
endif()
