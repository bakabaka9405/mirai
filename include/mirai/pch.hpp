#pragma once
#include <mirai/define.hpp>
#include <utility>
#include <iostream>
#include <iomanip>
#include <initializer_list>
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
#include <ranges>
#include <concepts>
#include <format>

#include <cassert>
#include <cmath>
#include <cctype>

#if MR_USE_MSVC
#include <immintrin.h>
#endif
namespace mirai {
	using namespace std::literals;

	namespace ranges = std::ranges; // NOLINT
	namespace views = std::views;	// NOLINT
	namespace chrono = std::chrono; // NOLINT

	using ll = long long;
	using ull = unsigned long long;
	using i64 = long long;
	using u64 = unsigned long long;

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
	using std::setprecision;
	using std::setw;

	// utility
	using std::make_pair;
	using std::make_tuple;
	using std::pair;
	using std::swap;
	using std::tuple;

	// string
	using std::format;
	using std::string;
	using std::string_view;
	using std::to_string;

	// iterator
	using std::back_inserter;
	using std::inserter;
	using std::istream_iterator;
	using std::ostream_iterator;

	// algorithm
	using std::copy;
	using std::copy_n;
	using std::sort;
	using std::stable_sort;

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
	using std::deque;
	using std::initializer_list;
	using std::map;
	using std::multimap;
	using std::multiset;
	using std::priority_queue;
	using std::queue;
	using std::set;
	using std::stack;
	using std::vector;

	// functor
	using std::function;
	using std::greater;
	using std::less;

	// concepts
	using std::floating_point;
	using std::integral;
} // namespace mirai