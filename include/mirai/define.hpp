#pragma once

#ifdef _MSC_VER
#define MR_USE_MSVC 1
#else
#define MR_USE_MSVC 0
#endif

#ifdef __clang__
#define MR_USE_CLANG 1
#else
#define MR_USE_CLANG 0
#endif

#ifdef __GNUC__
#define MR_USE_GCC 1
#else
#define MR_USE_GCC 0
#endif

#if MR_USE_MSVC || !defined(_WIN64)
#define MR_HAVE_INT128 0
#else
#define MR_HAVE_INT128 1
#endif

#if MR_USE_MSVC
#define MR_CPP_VERSION _MSVC_LANG
#else
#define MR_CPP_VERSION __cplusplus
#endif

#if (defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)) && defined(MR_DEBUG)
#define MR_HAVE_EXCEPTIONS 1
#else
#define MR_HAVE_EXCEPTIONS 0
#endif
#define MR_NODISCARD [[nodiscard]]

#define MR_NORETURN [[noreturn]]

#define MR_API inline

#define MR_LIKELY [[likely]]
#define MR_UNLIKELY [[unlikely]]

#if MR_USE_GCC || MR_USE_CLANG
#define MR_EXPECT(x) __builtin_expect(x, 1)
#define MR_UNEXPECT(x) __builtin_expect(x, 0)
#else
#define MR_EXPECT(x) (x)
#define MR_UNEXPECT(x) (x)
#endif

#define if_likely(x) if (MR_EXPECT(x))
#define if_unlikely(x) if (MR_UNEXPECT(x))

#ifdef MR_DEBUG
#define mr_noexcept noexcept(false)
#define MR_ASSERT(x) assert(x)
#define MR_TRY try
#define MR_CATCH catch
#else
#define mr_noexcept noexcept
#define MR_ASSERT(x) (void(0))
#define MR_TRY if (true)
#define MR_CATCH else if (false)
#endif

#define CONFIG_ITEM constexpr static auto&

#ifdef MR_DEBUG
#define MR_ASSUME(x) MR_ASSERT(x)
#else
#if __cplusplus >= 202300L
#define MR_ASSUME(x) [[assume(x)]]
#else
#if MR_USE_MSVC
#define MR_ASSUME(x) __assume(x)
#else
#define MR_ASSUME(x) \
	if (!(x)) __builtin_unreachable()
#endif
#endif
#endif

#define MR_NAMESPACE_BEGIN namespace mirai {
#define MR_NAMESPACE_END }