#pragma once

#include <cstddef>
#include <type_traits>
#include <vector>

#include "abs.hpp"

#include "../utils/template_helpers.hpp"

namespace terra::math
{
    // TODO this file will need cleaning a little and to work for more than just
    // a vector.

    template<typename T, terra::utils::enable_if_integer_t<T> = 0>
    inline T sum(const std::vector<T>& x)
    {
        T sum = x[0];

        for (size_t i = 1; i < x.size(); ++i)
        {
            sum += x[i];
        }

        return sum;
    }

    // Kahan and Babuska summation, Neumaier variant; accumulates less FP error
    template<typename T, terra::utils::enable_if_floating_t<T> = 0>
    inline T sum(const std::vector<T>& x)
    {
        T sum = x[0];
        T err = 0.0;

        for (size_t i = 1; i < x.size(); i++)
        {
            const T k = x[i];
            const T m = sum + k;
            err += abs(sum) >= abs(k) ? sum - m + k : k - m + sum;
            sum = m;
        }

        return sum + err;
    }
} // namespace terra::math