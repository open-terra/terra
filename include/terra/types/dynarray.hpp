#pragma once

#include <algorithm>
#include <cstddef>
#include <iterator>

namespace terra
{
    template<class T, class Alloc = std::allocator<T>>
    struct dynarray
    {
        typedef T value_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* iterator;
        typedef const T* const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;

        Alloc alloc;
        std::size_t length;
        T* buffer;

        std::size_t size() const
        {
            return length;
        }

        iterator begin()
        {
            return data();
        }
        const_iterator begin() const
        {
            return data();
        }
        const_iterator cbegin() const
        {
            return data();
        }
        iterator end()
        {
            return data() + size();
        }
        const_iterator end() const
        {
            return data() + size();
        }
        const_iterator cend() const
        {
            return data() + size();
        }

        reverse_iterator rbegin()
        {
            return {end()};
        }
        const_reverse_iterator rbegin() const
        {
            return {end()};
        }
        const_reverse_iterator crbegin() const
        {
            return {end()};
        }
        reverse_iterator rend()
        {
            return {begin()};
        }
        const_reverse_iterator rend() const
        {
            return {begin()};
        }
        const_reverse_iterator crend() const
        {
            return {begin()};
        }

        T& front()
        {
            return *begin();
        }
        T const& front() const
        {
            return *begin();
        }
        T& back()
        {
            return *(begin() + size() - 1);
        }
        T const& back() const
        {
            return *(begin() + size() - 1);
        }
        T* data()
        {
            return buffer;
        }
        T const* data() const
        {
            return buffer;
        }
        T& operator[](std::size_t i)
        {
            return data()[i];
        }
        T const& operator[](std::size_t i) const
        {
            return data()[i];
        }
        dynarray& operator=(dynarray&&) = default;
        dynarray(dynarray&&) = default;

        void allocate(size_t N)
        {
            this->length = N;
            buffer = this->alloc.allocate(N);
        }

        explicit dynarray(size_t N) : length(N), buffer(alloc.allocate(N))
        {
        }
        dynarray() : length(0), buffer(nullptr)
        {
        }
        dynarray(dynarray const& o) :
            alloc(), length(o.length), buffer(alloc.allocate(o.length))
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
            if (this->buffer)
            {
                this->alloc.deallocate(this->buffer, this->length);
            }
        }
    };
} // namespace terra
