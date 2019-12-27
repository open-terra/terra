#include <catch2/catch.hpp>

#include "terra/math.hpp"

using namespace terra::math;

TEST_CASE("abs", "[utils]")
{
    REQUIRE(abs(1) == 1);
    REQUIRE(abs(0) == 0);
    REQUIRE(abs(-1) == 1);

    REQUIRE(abs(1.0) == 1.0);
    REQUIRE(abs(0.0) == 0.0);
    REQUIRE(abs(-1.0) == 1.0);
}

TEST_CASE("ceil", "[utils]")
{
    REQUIRE(ceil<int>(1.0) == 1);
    REQUIRE(ceil<int>(0.1) == 1);
    REQUIRE(ceil<int>(0.0) == 0);
    REQUIRE(ceil<int>(-0.1) == 0);
    REQUIRE(ceil<int>(-1.0) == -1);
}

TEST_CASE("floor", "[utils]")
{
    REQUIRE(floor<int>(1.0) == 1);
    REQUIRE(floor<int>(0.1) == 0);
    REQUIRE(floor<int>(0.0) == 0);
    REQUIRE(floor<int>(-0.1) == -1);
    REQUIRE(floor<int>(-1.0) == -1);
}

TEST_CASE("mod", "[utils]")
{
    REQUIRE(mod<int>(3, 2) == 1);
    REQUIRE(mod<int>(2, 2) == 0);
    REQUIRE(mod<int>(1, 2) == 1);
}
