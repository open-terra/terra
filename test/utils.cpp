#include <catch2/catch.hpp>

#include "terra/utils/utils.hpp"

using namespace Utils;

TEST_CASE("FastAbs", "[Utils]")
{
    REQUIRE(FastAbs(1) == 1);
    REQUIRE(FastAbs(0) == 0);
    REQUIRE(FastAbs(-1) == 1);

    REQUIRE(FastAbs(1.0) == 1.0);
    REQUIRE(FastAbs(0.0) == 0.0);
    REQUIRE(FastAbs(-1.0) == 1.0);
}

TEST_CASE("FastCeil", "[Utils]")
{
    REQUIRE(FastCeil<int>(1.0) == 1);
    REQUIRE(FastCeil<int>(0.1) == 1);
    REQUIRE(FastCeil<int>(0.0) == 0);
    REQUIRE(FastCeil<int>(-0.1) == 0);
    REQUIRE(FastCeil<int>(-1.0) == -1);
}

TEST_CASE("FastFloor", "[Utils]")
{
    REQUIRE(FastFloor<int>(1.0) == 1);
    REQUIRE(FastFloor<int>(0.1) == 0);
    REQUIRE(FastFloor<int>(0.0) == 0);
    REQUIRE(FastFloor<int>(-0.1) == -1);
    REQUIRE(FastFloor<int>(-1.0) == -1);
}

TEST_CASE("FastMod", "[Utils]")
{
    REQUIRE(FastMod<int>(3, 2) == 1);
    REQUIRE(FastMod<int>(2, 2) == 0);
    REQUIRE(FastMod<int>(1, 2) == 1);
}
