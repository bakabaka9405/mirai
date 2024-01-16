#pragma once
#include <mirai/pch.hpp>
namespace mirai {
	[[maybe_unused]] inline constexpr ll
		pow10[] = { 1ll,
					10ll,
					100ll,
					1000ll,
					10000ll,
					100000ll,
					1000000ll,
					10000000ll,
					100000000ll,
					1000000000ll,
					10000000000ll,
					100000000000ll,
					1000000000000ll,
					10000000000000ll,
					100000000000000ll,
					1000000000000000ll,
					10000000000000000ll,
					100000000000000000ll,
					1000000000000000000ll };

	inline ull lg(ull n) mr_noexcept {
		return 63 - __builtin_clzll(n);
	}
} // namespace mirai

namespace std {
	MR_NODISCARD inline long long max(long long lhs, unsigned long long rhs) mr_noexcept {
#ifdef MR_DEBUG
#warning 正数与负数比大小
#endif
		return max(lhs, static_cast<long long>(rhs));
	}

	MR_NODISCARD inline long long max(unsigned long long lhs, long long rhs) mr_noexcept {
#ifdef MR_DEBUG
#warning 正数与负数比大小
#endif
		return max(static_cast<long long>(lhs), rhs);
	}

	MR_NODISCARD inline long long min(long long lhs, unsigned long long rhs) mr_noexcept {
#ifdef MR_DEBUG
#warning 正数与负数比大小
#endif
		return min(lhs, static_cast<long long>(rhs));
	}

	MR_NODISCARD inline long long min(unsigned long long lhs, long long rhs) mr_noexcept {
#ifdef MR_DEBUG
#warning 正数与负数比大小
#endif
		return min(static_cast<long long>(lhs), rhs);
	}
} // namespace std