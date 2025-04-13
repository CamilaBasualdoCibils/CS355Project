#include "Driver.hpp"
#include <mergesort.h>
std::chrono::milliseconds Driver::IotaIntTestTimed(uint32_t num_count, bool shuffled,uint32_t thread_count)
{
    std::vector<int> vec(num_count);
    std::iota(vec.begin(), vec.end(), 0);
    if (shuffled)
        std::shuffle(vec.begin(), vec.end(), std::mt19937{std::random_device{}()});

    return TestTimed(vec.begin(), vec.end());
}

std::chrono::milliseconds Driver::StringTestTimed(uint32_t num_strings, uint32_t stringLength, uint32_t max_thread_count)
{
    static const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr))); // Mersenne Twister RNG
    static std::uniform_int_distribution<> dist(0, charset.size() - 1);
    std::vector<std::string> strings;
    strings.resize(num_strings);
    for (int s = 0; s < num_strings; s++)
    {
        std::string &result = strings[s];
        result.reserve(stringLength);
        for (size_t i = 0; i < stringLength; ++i)
        {
            result += charset[dist(rng)];
        }
    }
    return TestTimed(strings.begin(), strings.end());
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

        [&]()
        { StringTestTimed(3, 3, 1); },
        [&]()
        { StringTestTimed(7, 5, 1); },
        [&]()
        { StringTestTimed(548, 85, 1); },
        [&]()
        { StringTestTimed(3, 1687, 1); },
        [&]()
        { StringTestTimed(1000, 1000, 1); },

        [&]()
        { StringTestTimed(3, 3, 3); },
        [&]()
        { StringTestTimed(7, 5, 3); },
        [&]()
        { StringTestTimed(548, 85, 3); },

        [&]()
        { StringTestTimed(3, 3, SYSTEM_THREAD_COUNT); },
        [&]()
        { StringTestTimed(7, 5, SYSTEM_THREAD_COUNT); },
        [&]()
        { StringTestTimed(548, 85, SYSTEM_THREAD_COUNT); },
        [&]()
        { StringTestTimed(1000, 1000, SYSTEM_THREAD_COUNT); },
        [&]()
        { StringTestTimed(10000, 10000, SYSTEM_THREAD_COUNT); },
        [&]()
        { StringTestTimed(3, 3, 64); },
        [&]()
        { StringTestTimed(7, 5, 64); },
        [&]()
        { StringTestTimed(548, 85, 64); },

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
