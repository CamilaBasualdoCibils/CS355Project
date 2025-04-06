#pragma once
#include "pch.hpp"
struct MergeSortSettings 
{

};
template <typename RandomAccessIterator>
void mergesortsinglethread(const MergeSortSettings& settings, RandomAccessIterator begin,RandomAccessIterator end);
template <typename RandomAccessIterator>
void MergeSingleThread(RandomAccessIterator begin, unsigned halfSize, unsigned backSize);


template<typename RandomAccessIterator>
inline void mergesortsinglethread(const MergeSortSettings& settings, RandomAccessIterator begin, RandomAccessIterator end)
{
    int size = std::distance(begin, end);
    if (size < 2)
        return;
    unsigned halfSize = size / 2;
    mergesortsinglethread(settings,begin, begin + halfSize);
    mergesortsinglethread(settings, begin + halfSize, end);

    MergeSingleThread(begin, halfSize,size - halfSize);
}

template<typename RandomAccessIterator>
inline void MergeSingleThread(RandomAccessIterator begin, unsigned halfSize, unsigned backSize)
{
    using ValueType = typename std::iterator_traits<RandomAccessIterator>::value_type;
    ValueType* merge = new ValueType[halfSize + backSize]{ 0 };
    
    unsigned i = 0, j = 0;
    while (i < halfSize && j < backSize)
    {
        if (begin[i] < begin[halfSize + j])
        {
            merge[i + j] = begin[i];
            ++i;
        }
        else
        {
            merge[i + j] = begin[halfSize + j];
            ++j;
        }
    }
    // Copy leftover elements from the left half
    while (i < halfSize) {
        merge[i + j] = *(begin + i);
        ++i;
    }
    // Copy leftover elements from the right half
    while (j < backSize) {
        merge[i + j] = *(begin + halfSize + j);
        ++j;
    }
    std::copy(merge, merge + (halfSize + backSize), begin);
    delete[] merge;
}
