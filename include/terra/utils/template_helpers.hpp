#pragma once

#include <type_traits>

namespace terra::utils
{
    template<typename T>
    using enable_if_integer_t =
        typename std::enable_if<std::is_integral<T>::value, int>::type;

    template<typename T>
    using enable_if_floating_t =
        typename std::enable_if<std::is_floating_point<T>::value, int>::type;

    template<auto l, auto r>
    using enable_if_value_equal_t = typename std::enable_if<l == r, int>::type;

    template<typename L, typename R>
    using enable_if_type_equal_t =
        typename std::enable_if<std::is_same<L,R>::value, int>::type;
} // namespace terra::utils