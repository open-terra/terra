#pragma once

#include <span>

#include "concepts.hpp"

namespace terra
{
    template<class T, class Array>
    requires terra::Container<Array, T>
    constexpr auto to_span(const Array& a) -> std::span<T>
    {
        /*
        if constexpr (std::is_const<T>::value)
        {
            return std::span(std::data(a), std::size(a));
        }
        else
        {
            return std::span(const_cast<T*>(std::data(a)), std::size(a));
        }
        */

        return std::span(const_cast<T*>(std::data(a)), std::size(a));
    }
} // namespace terra