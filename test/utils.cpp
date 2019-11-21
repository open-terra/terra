#include <catch2/catch.hpp>

#include "terra/utils/utils.hpp"

using namespace utils;

TEST_CASE("fast_abs", "[utils]")
{
    REQUIRE(fast_abs(1) == 1);
    REQUIRE(fast_abs(0) == 0);
    REQUIRE(fast_abs(-1) == 1);

    REQUIRE(fast_abs(1.0) == 1.0);
    REQUIRE(fast_abs(0.0) == 0.0);
    REQUIRE(fast_abs(-1.0) == 1.0);
}

TEST_CASE("fast_ceil", "[utils]")
{
    REQUIRE(fast_ceil<int>(1.0) == 1);
    REQUIRE(fast_ceil<int>(0.1) == 1);
    REQUIRE(fast_ceil<int>(0.0) == 0);
    REQUIRE(fast_ceil<int>(-0.1) == 0);
    REQUIRE(fast_ceil<int>(-1.0) == -1);
}

TEST_CASE("fast_floor", "[utils]")
{
    REQUIRE(fast_floor<int>(1.0) == 1);
    REQUIRE(fast_floor<int>(0.1) == 0);
    REQUIRE(fast_floor<int>(0.0) == 0);
    REQUIRE(fast_floor<int>(-0.1) == -1);
    REQUIRE(fast_floor<int>(-1.0) == -1);
}

TEST_CASE("fast_mod", "[utils]")
{
    REQUIRE(fast_mod<int>(3, 2) == 1);
    REQUIRE(fast_mod<int>(2, 2) == 0);
    REQUIRE(fast_mod<int>(1, 2) == 1);
}
