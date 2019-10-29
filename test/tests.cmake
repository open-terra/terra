add_executable(terra_tests
    test/utils.cpp
    test/main.cpp
)
target_link_libraries(terra_tests Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(terra_tests)