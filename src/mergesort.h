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
    const unsigned totalSize = halfSize + backSize;

    // Allocate a temporary buffer. Use unique_ptr to manage the memory automatically.
    std::unique_ptr<ValueType[]> merge(new ValueType[totalSize]);

    // Create raw pointers for merging.
    ValueType* left = &begin[0];
    ValueType* right = &begin[halfSize];
    unsigned i = 0, j = 0, k = 0;

    // Merge while both halves have elements.
    while (i < halfSize && j < backSize)
    {
        // Use conditional operator to reduce branching overhead.
        merge[k++] = (left[i] < right[j]) ? left[i++] : right[j++];
    }

    // Copy any leftover elements from the left half.
    if (i < halfSize)
    {
        std::copy(left + i, left + halfSize, merge.get() + k);
    }
    // Or copy the ones left from the right half.
    else if (j < backSize)
    {
        std::copy(right + j, right + backSize, merge.get() + k);
    }

    // Copy back the merged sequence into the original array.
    std::copy(merge.get(), merge.get() + totalSize, begin);
}
template<typename RandomAccessIterator>
void MergeMultiThread(RandomAccessIterator begin, unsigned halfSize, unsigned backSize, int depth = 0)
{
    using ValueType = typename std::iterator_traits<RandomAccessIterator>::value_type;
    unsigned totalSize = halfSize + backSize;

    // Threshold and depth limit for spawning parallel tasks.
    const unsigned parallelThreshold = 10000; // Only parallelize if the total size is above this.
    const int maxDepth = 2;                   // Limit the recursion depth for parallel merging.

    // If the problem is small or we've reached the maximum recursion depth,
    // fall back to the sequential merge.
    if (totalSize < parallelThreshold || depth >= maxDepth)
    {
        MergeSingleThread(begin, halfSize, backSize);
        return;
    }

    // Partition the merge:
    // Choose a pivot in the left half (middle element).
    unsigned leftMid = halfSize / 2;
    ValueType pivot = *(begin + leftMid);

    // In the right half, find the insertion point for the pivot using binary search.
    RandomAccessIterator rightBegin = begin + halfSize;
    unsigned rightMid = static_cast<unsigned>(std::lower_bound(rightBegin, begin + totalSize, pivot) - rightBegin);

    // Allocate a temporary buffer for the merged output.
    std::unique_ptr<ValueType[]> temp(new ValueType[totalSize]);

    // Define a lambda that merges a specific range into the temporary buffer.
    // The lambda parameters define the subrange indices for left and right segments,
    // and where the merged output should be placed in the temporary buffer.
    auto merge_range = [begin, &temp](unsigned leftStart, unsigned leftEnd,
        unsigned rightStart, unsigned rightEnd,
        unsigned destStart)
        {
            unsigned i = leftStart, j = rightStart, k = destStart;
            while (i < leftEnd && j < rightEnd)
            {
                temp[k++] = (*(begin + i) < *(begin + j)) ? *(begin + i++) : *(begin + j++);
            }
            while (i < leftEnd)
            {
                temp[k++] = *(begin + i++);
            }
            while (j < rightEnd)
            {
                temp[k++] = *(begin + j++);
            }
        };

    // Launch two concurrent tasks to merge the two partitions:
    // - Left merge: merge the left part of the left segment [0, leftMid)
    //               with the corresponding part of the right segment [halfSize, halfSize+rightMid).
    // - Right merge: merge the remaining portions.
    std::future<void> futureLeft = std::async(std::launch::async, merge_range,
        0u, leftMid,              // left segment indices
        halfSize, halfSize + rightMid, // right segment indices
        0u);                      // destination index starts at 0

    std::future<void> futureRight = std::async(std::launch::async, merge_range,
        leftMid, halfSize,         // left segment indices
        halfSize + rightMid, totalSize, // right segment indices
        leftMid + rightMid);       // destination index

    // Wait for both merging tasks to complete.
    futureLeft.get();
    futureRight.get();

    // Copy the merged data from the temporary buffer back to the original sequence.
    std::copy(temp.get(), temp.get() + totalSize, begin);
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
    MergeMultiThread(begin, halfSize, size - halfSize);
}
template <typename RandomAccessIterator>
void mergesortparallel_entry(const MergeSortSettings& settings,
                             RandomAccessIterator begin,
                             RandomAccessIterator end)
{
    std::atomic<int> activeThreads(1); // We start with 1 "active" thread
    mergesortparallel(settings, begin, end, activeThreads);
    // No need to wait here specifically, because mergesortparallel
    // waits/join()s on the threads it spawns before returning.
}