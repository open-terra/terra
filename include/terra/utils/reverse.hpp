#pragma once

namespace terra::utils
{
    template <typename T>
    struct reverse_wrapper { const T* iterable; };

    template <typename T>
    auto begin (reverse_wrapper<T> w) { return std::rbegin(*w.iterable); }

    template <typename T>
    auto end (reverse_wrapper<T> w) { return std::rend(*w.iterable); }

    template <typename T>
    constexpr auto reverse(const T& i)
    {
        return reverse_wrapper<T>{&i};
    }
} // namespace terra::utils