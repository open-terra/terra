#pragma once

#include <span>

#include "concepts.hpp"

namespace terra
{
    template<class T, class Array> requires terra::Container<Array, T>
    constexpr auto to_span(Array& a) -> std::span<T, std::dynamic_extent>
    {
        return std::span(std::data(a), std::size(a));
    }

    template<class T, class Array> requires terra::Container<Array, T>
    constexpr auto to_span(const Array& a) -> std::span<const T, std::dynamic_extent>
    {
        return std::span(std::data(a), std::size(a));
    }
} // namespace terra