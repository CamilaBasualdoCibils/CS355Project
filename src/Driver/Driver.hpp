#include <pch.hpp>

class Driver
{
    std::chrono::milliseconds IotaIntTestTimed(uint32_t num_count, bool shuffled);
    template <typename _RAIter>
    std::chrono::milliseconds IntTestTimed(_RAIter begin, _RAIter end);

    template <typename _RAIter>
    std::chrono::milliseconds OurMergeSortTimed(const MergeSortSettings &sett, _RAIter begin, _RAIter end);

public:
    Driver();
    void RunAllTests();
    void RunTest(std::uint32_t test_index);

private:
    std::vector<std::function<void(void)>> tests;
};

template <typename _RAIter>
inline std::chrono::milliseconds Driver::IntTestTimed(_RAIter begin, _RAIter end)
{
    MergeSortSettings settings;
    std::stringstream orig, after;
    orig << "Original: ";
    for (auto it = begin; it != end; it++)
        orig << *it << " ";
    orig << "\n";

    auto duration = OurMergeSortTimed(settings, begin, end);
    const bool sorted = std::ranges::is_sorted(begin, end);

    if (!sorted)
    {
        after << "After: ";
        for (auto it = begin; it != end; it++)
            after << *it << " ";
        after << "\n";
        std::cout << orig.str() << after.str();
        
        DEBUG_BREAK(); //FAILURE BREAKPOINT
    }
    
    assert(sorted && "WRONG RUN");
    return duration;
}

template <typename _RAIter>
inline std::chrono::milliseconds Driver::OurMergeSortTimed(const MergeSortSettings &sett, _RAIter begin, _RAIter end)
{
    auto start_t = std::chrono::high_resolution_clock::now();
    //std::stable_sort(begin, end);
     MergeSort(sett, begin, end);
    auto end_t = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(end_t - start_t);
}
