#include "Driver.hpp"
#include <mergesort.h>
std::chrono::milliseconds Driver::IotaIntTestTimed(uint32_t num_count, bool shuffled)
{
    std::vector<int> vec(num_count);
    std::iota(vec.begin(), vec.end(), 0);
    if (shuffled)
        std::shuffle(vec.begin(), vec.end(), std::mt19937{std::random_device{}()});

    return IntTestTimed(vec.begin(), vec.end());
}

Driver::Driver()
{
    tests = {
        [&]()
        { IotaIntTestTimed(3, false); },
        [&]()
        { IotaIntTestTimed(3, true); },
        [&]()
        { IotaIntTestTimed(4, false); },
        [&]()
        { IotaIntTestTimed(4, true); },

        [&]()
        { IotaIntTestTimed(15, true); },
        [&]()
        { IotaIntTestTimed(30, true); },
        [&]()
        { IotaIntTestTimed(60, true); },
        [&]()
        { IotaIntTestTimed(90, true); },
    };
}

void Driver::RunAllTests()
{
    for (const auto f : tests)
        f();
}

void Driver::RunTest(std::uint32_t test_index)
{
    assert(test_index < tests.size() && "INVALID TEST INDEX");
    tests[test_index]();
}
