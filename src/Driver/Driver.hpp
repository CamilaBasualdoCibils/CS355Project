#include <pch.hpp>
#include <mergesort.h>
#include <thread>

using RecordTimeType = std::chrono::nanoseconds;
class Driver
{

 

public:
RecordTimeType IotaIntTestTimed(uint32_t num_count, bool shuffled,uint32_t thread_count = 1);
template <typename _RAIter>
RecordTimeType TestTimed(_RAIter begin, _RAIter end,uint32_t thread_count = 1);

template <typename _RAIter>
RecordTimeType OurMergeSortTimed(const MergeSortSettings &sett, _RAIter begin, _RAIter end);

RecordTimeType StringTestTimed(uint32_t num_strings, uint32_t stringLength, uint32_t max_thread_count = 1);
    Driver();
    void RunAllTests();
    void RunTest(std::uint32_t test_index);

private:
    const static inline uint32_t SYSTEM_THREAD_COUNT = std::thread::hardware_concurrency();
    std::vector<std::function<void(void)>> tests;
};

template <typename _RAIter>
inline RecordTimeType Driver::TestTimed(_RAIter begin, _RAIter end,uint32_t thread_count)
{
    MergeSortSettings settings;
    settings.maxThreads = thread_count;
    std::stringstream orig, after;
    orig << "Original: ";
    for (auto it = begin; it != end; it++)
        orig << *it << " ";
    orig << "\n";

    auto duration = OurMergeSortTimed(settings, begin, end);
    const bool sorted = std::ranges::is_sorted(begin, end);

    if (!sorted)
    {
        after << "After:    ";
        for (auto it = begin; it != end; it++)
            after << *it << " ";
        after << "\n";
        std::cout << orig.str() << after.str();

        DEBUG_BREAK(); // FAILURE BREAKPOINT
    }

    assert(sorted && "WRONG RUN");
    return duration;
}

template <typename _RAIter>
inline RecordTimeType Driver::OurMergeSortTimed(const MergeSortSettings &sett, _RAIter begin, _RAIter end)
{
    auto start_t = std::chrono::high_resolution_clock::now();
    // std::stable_sort(begin,end);
    mergesortparallel_entry(sett, begin, end);
    auto end_t = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<RecordTimeType>(end_t - start_t);
}
