#pragma once
#include <mirai/define.hpp>
#include <utility>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <initializer_list>
#include <type_traits>
#include <queue>
#include <stack>
#include <string>
#include <string_view>
#include <array>
#include <vector>
#include <map>
#include <set>
#include <random>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iterator>
#include <chrono>
#include <regex>
#include <ranges>
#include <concepts>
#include <memory_resource>
#include <bitset>
#include <bit>
#include <numbers>
#include <coroutine>
#include <exception>
#include <cassert>
#include <cmath>
#include <cctype>
#include <cstring>
#include <stdexcept>
#include <source_location>
#ifdef _WIN32
#include <conio.h>
#endif

#if MR_USE_MSVC
#include <intrin.h>
#include <immintrin.h>
#pragma intrinsic(_umul128)
#endif
MR_NAMESPACE_BEGIN

using namespace std::literals;

namespace ranges = std::ranges; // NOLINT
namespace views = std::views;	// NOLINT
namespace chrono = std::chrono; // NOLINT

using i8 = int8_t;
using u8 = uint8_t;
using i32 = int32_t;
using u32 = uint32_t;
using ll = int64_t;
using ull = uint64_t;
using i64 = int64_t;
using u64 = uint64_t;

#if MR_HAVE_INT128
using i128 = __int128_t;
using u128 = __uint128_t;
#endif

// IO
using std::cerr;
using std::cin;
using std::clog;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::setprecision;
using std::setw;
#ifdef _WIN32
#define getchar_unlocked _getchar_nolock
#define putchar_unlocked _putchar_nolock
#define fread_unlocked _fread_nolock
#define fwrite_unlocked _fwrite_nolock
#define flockfile(f) _lock_file(f)
#define funlockfile(f) _unlock_file(f)
#endif
// utility
using std::copy;
using std::copy_n;
using std::declval;
using std::exchange;
using std::make_pair;
using std::make_tuple;
using std::pair;
using std::swap;
using std::tuple;

// bit
using std::popcount;

// string
using std::string;
using std::string_view;
using std::to_string;

// iterator
using std::back_inserter;
using std::inserter;
using std::istream_iterator;
using std::ostream_iterator;

// tuple
using std::make_pair;
using std::make_tuple;
using std::tie;

// algorithm
using std::nth_element;
using std::sort;
using std::stable_sort;
using std::unique;
using std::unique_copy;

// random
using std::default_random_engine;
using std::random_device;
using std::uniform_int_distribution;
using std::uniform_real_distribution;

// chrono
using chrono::duration;
using chrono::steady_clock;
using chrono::system_clock;
using chrono::time_point;

// numeric
using std::adjacent_difference;
using std::gcd;
using std::iota;
using std::lcm;
using std::lerp;
using std::max;
using std::min;
using std::partial_sum;

// data structure
using std::array;
using std::bitset;
using std::deque;
using std::initializer_list;
using std::map;
using std::multimap;
using std::multiset;
using std::priority_queue;
using std::queue;
using std::set;
using std::vector;
template <typename T>
using stack = std::stack<T, vector<T>>;

// functor
using std::function;
using std::greater;
using std::less;

// regex
using std::regex;
using std::regex_search;
using std::smatch;

// concepts
using std::floating_point;
using std::integral;

// meta programming
using std::forward;
using std::move;

#define _MR_COUNT_ARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...) N
#define _MR_COUNT_ARGS(...) _MR_COUNT_ARGS_IMPL(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)
#define _MR_CONCAT_IMPL(x, y) x##y
#define _MR_CONCAT(x, y) _MR_CONCAT_IMPL(x, y)

#define _MR_LAMBDA1(x) [&] mr_noexcept { return x; }
#define _MR_LAMBDA2(x, y) [&](auto&&(x)) mr_noexcept { return y; }
#define _MR_LAMBDA3(x, y, z) [&](auto&&(x), auto&&(y)) mr_noexcept { return z; }
#define _MR_LAMBDA4(x, y, z, w) [&](auto&&(x), auto&&(y), auto&&(z)) mr_noexcept { return w; }
#define lambda(...) _MR_CONCAT(_MR_LAMBDA, _MR_COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)

MR_NAMESPACE_END