#include <catch2/catch.hpp>

#include "terra/math.hpp"

using namespace terra::math;

TEST_CASE("abs", "[math]")
{
    REQUIRE(abs(1) == 1);
    REQUIRE(abs(0) == 0);
    REQUIRE(abs(-1) == 1);

    REQUIRE(abs(1.0) == 1.0);
    REQUIRE(abs(0.0) == 0.0);
    REQUIRE(abs(-1.0) == 1.0);
}

TEST_CASE("ceil", "[math]")
{
    REQUIRE(ceil<int>(1.0) == 1);
    REQUIRE(ceil<int>(0.1) == 1);
    REQUIRE(ceil<int>(0.0) == 0);
    REQUIRE(ceil<int>(-0.1) == 0);
    REQUIRE(ceil<int>(-1.0) == -1);
}

TEST_CASE("floor", "[math]")
{
    REQUIRE(floor<int>(1.0) == 1);
    REQUIRE(floor<int>(0.1) == 0);
    REQUIRE(floor<int>(0.0) == 0);
    REQUIRE(floor<int>(-0.1) == -1);
    REQUIRE(floor<int>(-1.0) == -1);
}

TEST_CASE("mod", "[math]")
{
    REQUIRE(mod<int>(3, 2) == 1);
    REQUIRE(mod<int>(2, 2) == 0);
    REQUIRE(mod<int>(1, 2) == 1);
}

TEST_CASE("round", "[math]")
{
    REQUIRE(round<int>(1.23) == 1);
    REQUIRE(round<int>(1.5) == 2);
    REQUIRE(round<int>(1.87) == 2);
}

TEST_CASE("sum", "[math]")
{
    std::vector<int> test_ints = {1, 2, -1};
    REQUIRE(sum<int>(test_ints) == 2);

    std::vector<double> test_floats = {1.2, 2.3, 7.0};
    REQUIRE(sum<double>(test_floats) == 10.5);
}
