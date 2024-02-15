#pragma once
//#define MR_FLAG_USE_INDEPENDENT_FMT
#ifndef MR_LOCAL
#pragma GCC optimize("Ofast,no-stack-protector,unroll-loops,fast-math,lto,inline,omit-frame-pointer,unsafe-math-optimizations,unsafe-loop-optimizations,tree-vectorize,no-exceptions,no-rtti")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4.1,sse4.2,avx,avx2,popcnt,tune=native")
#endif
#include <mirai/oj/gcc_optimize_setting.hpp>
#include <mirai/util/pipeline.hpp>
#include <mirai/util/debug.hpp>
#include <mirai/util/io.hpp>