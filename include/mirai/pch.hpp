#pragma once
#include <mirai/define.hpp>
#include <iostream>
#include <iomanip>
#include <queue>
#include <stack>
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include <algorithm>
#include <numeric>
#include <functional>
#include <chrono>
#include <ranges>
namespace mirai{
	using namespace std::literals;

	namespace ranges=std::ranges;	//NOLINT
	namespace views=std::views;		//NOLINT
	namespace chrono=std::chrono;	//NOLINT

	using ll=long long;
	using ull=unsigned long long;
	using i64=long long;
	using i128=__int128;

	using std::cin;
	using std::cout;
	using std::endl;
	using std::setw;
	using std::setprecision;
	using std::string;
	using std::string_view;
	using std::vector;
	using std::map;
	using std::multimap;
	using std::set;
	using std::multiset;
	using std::unordered_map;
	using std::unordered_multimap;
	using std::unordered_set;
	using std::unordered_multiset;
	using std::queue;
	using std::deque;
	using std::stack;
	using std::priority_queue;
	using std::default_random_engine;
	using std::uniform_int_distribution;
	using std::uniform_real_distribution;
	using std::random_device;
	using std::function;
	using std::iota;
	using std::sort;
	using std::greater;
	using std::less;
}