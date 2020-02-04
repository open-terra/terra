#pragma once

#include <algorithm>
#include <cstdint>
#include <exception>
#include <memory>
#include <vector>

#include "../base_types.hpp"
#include "vec2.hpp"

namespace terra
{
    class hash_grid
    {
        class double_index_error : public std::exception
        {
            virtual const char* what() const throw()
            {
                return "A value already exists at this location in the "
                       "hashtable";
            }
        };

        const tfloat sqrt1_2 = 0.7071067811865476;

        int64_t grid_size_x;
        int64_t grid_size_y;
        int64_t n;
        tfloat bucket_size;

        std::vector<int64_t> hashtable;

    public:
        hash_grid();
        hash_grid(int64_t size_x, int64_t size_y, tfloat radius);

        void set(const terra::vec2& point, int64_t index);
        int64_t at(const terra::vec2& point);

        std::vector<int64_t> neighbours(const terra::vec2& point);
    };
} // namespace terra