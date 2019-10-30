#pragma once

#include <algorithm>
#include <cstdint>
#include <exception>
#include <memory>
#include <vector>

#include "../types/vector_2.hpp"
#include "../utils/utils.hpp"

namespace Terra
{
    class HashGrid
    {
        class double_index_error : public std::exception
        {
            virtual const char* what() const throw()
            {
                return "A value already exists at this location in the hashtable";
            }
        };

        int64_t gridSizeX;
        int64_t gridSizeY;
        int64_t neighbours;
        double bucketSize;

        std::vector<int64_t> hashtable;

    public:
        HashGrid();
        HashGrid(int64_t sizeX, int64_t sizeY, double radius);

        void Set(const Terra::Vector2 &point, int64_t index);
        int64_t At(const Terra::Vector2 &point);

        std::vector<int64_t> Neighbours(const Terra::Vector2& point);
    };
}