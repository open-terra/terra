#pragma once

#ifdef _MSC_VER
#    define __cpp_lib_concepts
#endif // _MSC_VER

#include <concepts>

namespace terra
{
    template<class T>
    concept Integral = std::is_integral<T>::value;

    template<class T>
    concept Real = std::is_floating_point<T>::value;

    template<typename T, typename U>
    concept Container = requires(T a)
    {
        std::data(a);
        std::size(a);
        std::same_as<typename T::value_type, U>;
    };

    template<typename T>
    concept IntContainer = requires(T a)
    {
        std::data(a);
        std::size(a);
    }
    &&Integral<T>;

    template<typename T>
    concept RealContainer = requires(T a)
    {
        std::data(a);
        std::size(a);
    }
    &&Real<T>;

    template<typename T>
    concept Vec2 = requires(typename T::point_type p)
    {
        typename T::value_type;
        T::get_x(p);
        T::get_y(p);
        T::get_dist2(p, p);
    };
} // namespace terra
