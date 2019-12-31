#pragma once

#include <type_traits>

namespace terra::utils
{
    template<typename T>
    using enable_if_integer_t = typename std::enable_if<std::is_integral<T>::value, int>::type;

    template<typename T>
    using enable_if_floating_t = typename std::enable_if<std::is_floating_point<T>::value, int>::type;
}