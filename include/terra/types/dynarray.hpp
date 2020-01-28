#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>

namespace terra
{
    template<typename T>
    struct dynarray
    {
        typedef       T                               value_type;
        typedef       T&                              reference;
        typedef const T&                              const_reference;
        typedef       T*                              iterator;
        typedef const T*                              const_iterator;
        typedef std::reverse_iterator<iterator>       reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef size_t                                size_type;
        typedef ptrdiff_t                             difference_type;

        std::size_t length;
        T* buffer;

        std::size_t size() const { return length; }

        iterator begin() { return data(); }
        const_iterator begin() const { return data(); }
        const_iterator cbegin() const { return data(); }
        iterator end() { return data()+size(); }
        const_iterator end() const { return data()+size(); }
        const_iterator cend() const { return data()+size(); }

        reverse_iterator rbegin() { return {end()}; }
        const_reverse_iterator rbegin() const { return {end()}; }
        const_reverse_iterator crbegin() const { return {end()}; }
        reverse_iterator rend() { return {begin()}; }
        const_reverse_iterator rend() const { return {begin()}; }
        const_reverse_iterator crend() const { return {begin()}; }

        T& front() { return *begin(); }
        T const& front() const { return *begin(); }
        T& back() { return *(begin()+size()-1); }
        T const& back() const { return *(begin()+size()-1); }
        T* data() { return buffer; }
        T const* data() const { return buffer; }
        T& operator[]( std::size_t i ) { return data()[i]; }
        T const& operator[]( std::size_t i ) const { return data()[i]; }
        dynarray& operator=(dynarray &&) = default;
        dynarray(dynarray &&) = default;

        explicit dynarray(std::size_t N) : length(N), buffer(new T[length]) {}
        dynarray():length(0), buffer() {}
        dynarray(dynarray const& o) : length(o.N), buffer(new T[length])
        {
            std::copy(o.begin(), o.end(), this->begin());
        }
        dynarray(std::initializer_list<T> l) :
            length(l.size()), buffer(new T[length])
        {
            std::copy(l.begin(), l.end(), this->begin());
        }
        ~dynarray()
        {
            delete[] buffer;
        }
    };
} // namespace terra
