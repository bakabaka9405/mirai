#pragma once

#define MR_NODISCARD [[nodiscard]]

#ifdef MR_DEBUG
#define mr_noexcept noexcept(false)
#define mr_assert(x) assert(x)
#else
#define mr_noexcept noexcept
//#define mr_assert(x) [[assume(!(x))]]
#define mr_assert(x) void()
#endif