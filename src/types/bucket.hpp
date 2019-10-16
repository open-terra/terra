#pragma once

#include <cstdint>
#include <initializer_list>

namespace Terra
{
    template <typename T, size_t Size>
    struct Bucket
    {
    private:
        size_t current_index;
        T data[Size];

    public:
        Bucket();
        Bucket(std::initializer_list<T> data);

        inline bool Empty();
        inline bool Full();

        inline size_t Length();

        inline void Push(const T& val);
        inline void Remove(size_t pos);

        inline T At(size_t pos);

        T& operator[](size_t pos);
        const T& operator[](size_t pos) const;
    };
}