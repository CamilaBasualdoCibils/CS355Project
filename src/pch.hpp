
#pragma once


#include <algorithm>
#include <cstring>
#include <iterator> 
#include <iostream>
#include <cstdint>
#include <vector>
#include <ranges>
#include <numeric>
#include <random>
#include <chrono>
#include <assert.h>
#include <functional>
#include <type_traits>
#include <sstream>
#include <utility>
#include <thread>
#include <atomic>
#if defined(_MSC_VER)
  #include <intrin.h>
  #define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
  #include <csignal>
  #define DEBUG_BREAK() raise(SIGTRAP)
#else
  #define DEBUG_BREAK() ((void)0)
#endif