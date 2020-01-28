#include <catch2/catch.hpp>

#include <cstdint>

#include "terra/types/dynarray.hpp"

TEST_CASE("explicit constructor", "[dynarray]")
{
    size_t len = 32;
    terra::dynarray<uint32_t> array(len);
    REQUIRE(array.size() == len);
    array[2] = 5;
    REQUIRE(array[2] == 5);
}

TEST_CASE("initialiser list", "[dynarray]")
{
    terra::dynarray<uint32_t> array = {1, 2, 3, 4, 5};
    REQUIRE(array.size() == 5);
    REQUIRE(array[2] == 3);
    array[2] = 5;
    REQUIRE(array[2] == 5);
}
