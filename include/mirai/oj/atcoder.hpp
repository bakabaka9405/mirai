#pragma once
// #define MR_FLAG_USE_INDEPENDENT_FMT
#ifndef MR_LOCAL
#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC optimize("Ofast,no-stack-protector,unroll-loops,lto,fast-math,inline,omit-frame-pointer,unsafe-math-optimizations,unsafe-loop-optimizations,tree-vectorize")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4.1,sse4.2,avx,avx2,popcnt,tune=native")
#endif
#include <mirai/util/pipe.hpp>
#include <mirai/util/debug.hpp>
#include <mirai/util/io.hpp>