#pragma once
#ifndef MR_LOCAL
#pragma GCC target("sse,sse2,sse3,ssse3,sse4.1,sse4.2,avx,avx2,popcnt,tune=native")
#endif
#include <mirai/util/pipe.hpp>
#include <mirai/util/debug.hpp>
#include <mirai/util/io.hpp>