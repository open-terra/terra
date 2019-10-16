#include "bucket.hpp"

#include <cstring>
#include <exception>
#include <stdexcept>

using namespace Terra;

template <typename T, size_t Size>
Bucket<T, Size>::Bucket() : current_index(0) {}

template <typename T, size_t Size>
Bucket<T, Size>::Bucket(std::initializer_list<T> data) : current_index(0)
{
    for (const T& value : data)
    {
        this->data[current_index] = value;
        current_index++;
    }
}

template <typename T, size_t Size>
inline bool Bucket<T, Size>::Empty()
{
    return this->current_index == 0;
}

template <typename T, size_t Size>
inline bool Bucket<T, Size>::Full()
{
    return !(this->current_index < Size - 1);
}

template <typename T, size_t Size>
inline size_t Bucket<T, Size>::Length()
{
    return this->count + 1;
}

template <typename T, size_t Size>
inline void Bucket<T, Size>::Push(const T& val)
{
    if (this->current_index >= Size)
    {
        throw std::exception("tried to push a value to the bucket when the bucket is full");
    }

    this->data[this->current_index] = val;
    this->current_index++;
}

template <typename T, size_t Size>
inline void Bucket<T, Size>::Remove(size_t pos)
{
    if (!(pos < Size))
    {
        throw std::out_of_range("index out of range");
    }

    if (!(pos <= this->current_index))
    {
        throw std::out_of_range("index not yet used");
    }

    if (pos != this->current_index)
    {
        size_t dest = pos * sizeof(T);
        size_t begin = dest + sizeof(T);
        size_t size = (this->current_index * sizeof(T)) - begin;

        std::memcpy(dest, begin, size);
    }

    this->current_index--;
}

template <typename T, size_t Size>
inline T Bucket<T, Size>::At(size_t pos)
{
    if (!(pos < Size))
    {
        throw std::out_of_range("index out of range");
    }

    if (!(pos < this->current_index))
    {
        throw std::out_of_range("index not yet used");
    }

    return data[pos];
}

template <typename T, size_t Size>
T& Bucket<T, Size>::operator[](size_t pos)
{
    return this->data[pos];
}

template <typename T, size_t Size>
const T& Bucket<T, Size>::operator[](size_t pos) const
{
    return this->data[pos];
}
