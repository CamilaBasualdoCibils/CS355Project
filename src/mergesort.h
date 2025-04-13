#pragma once
#include "pch.hpp"
struct MergeSortSettings 
{
     int maxThreads;
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
    ValueType* merge = new ValueType[halfSize + backSize]{ ValueType() };
    
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
template <typename RandomAccessIterator>
void mergesortparallel(const MergeSortSettings& settings,
    RandomAccessIterator begin,
    RandomAccessIterator end,
    std::atomic<int>& activeThreads)
{
    // Base case: if the range is 0 or 1 elements, it's already sorted
    int size = static_cast<int>(std::distance(begin, end));
    if (size < 2) {
        return;
    }

    // Calculate midpoint
    int halfSize = size / 2;
    RandomAccessIterator mid = begin + halfSize;
    int fetchActiveThreads = activeThreads.load(std::memory_order_relaxed);

    // Decide whether to spawn a new thread for the left half
    if (fetchActiveThreads < settings.maxThreads && activeThreads.compare_exchange_weak(fetchActiveThreads, fetchActiveThreads + 1))
    {
        // We can increment the activeThreads and spawn a new thread
        activeThreads.fetch_add(1, std::memory_order_relaxed);

        // Spawn a thread for the left half
        auto thr_func = [&settings, begin, mid, &activeThreads]()
            {
                mergesortparallel(settings, begin, mid, activeThreads);
            };
        std::thread leftThread(thr_func);

        // Meanwhile, handle the right half in the current thread
        mergesortparallel(settings, mid, end, activeThreads);

        // Join the left half
        leftThread.join();
        activeThreads.fetch_sub(1, std::memory_order_relaxed);
    }
    else
    {
        // If we're at the max thread limit, do both halves on this thread
        mergesortparallel(settings, begin, mid, activeThreads);
        mergesortparallel(settings, mid, end, activeThreads);
    }

    // Finally, merge both halves
    MergeSingleThread(begin, halfSize, size - halfSize);
}
