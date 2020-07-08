#pragma once

#include <cstddef>

#include "../concepts.hpp"
#include "abs.hpp"
#include "../types/dynarray.hpp"
#include "../utils/template_helpers.hpp"

namespace terra::math
{
    // TODO this file will need cleaning a little and to work for more than just
    // a vector.

    template<class T, class Array> requires terra::Container<Array, T>
    inline T sum(const Array& x)
    {
        T sum = x[0];
        if constexpr (std::is_integral<T>::value)
        {
            for (size_t i = 1; i < x.size(); ++i)
            {
                sum += x[i];
            }
        }
        else if constexpr (std::is_floating_point<T>::value)
        {
            // Kahan and Babuska summation, Neumaier variant; accumulates less
            // FP error
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
        else
        {
            static_assert(true, "unsummable type");
        }

        return sum;
    }
} // namespace terra::math