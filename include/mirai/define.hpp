#pragma once

#define MR_NODISCARD [[nodiscard]]

#define MR_NORETURN [[noreturn]]

#ifdef _MSC_VER
#define MR_USE_MSVC 1
#else
#define MR_USE_MSVC 0
#endif

#if MR_USE_MSVC
#define MR_HAVE_INT128 0
#else
#define MR_HAVE_INT128 1
#endif

#ifdef MR_DEBUG
#define mr_noexcept noexcept(false)
#define mr_assert(x) assert(x)
#else
#define mr_noexcept noexcept
// #define mr_assert(x) [[assume(!(x))]]
#define mr_assert(x) (void(0))
#endif

#define CONFIG_ITEM constexpr static auto&