#pragma once
#include <mirai/pch.hpp>
namespace mirai {
	MR_NODISCARD inline ll clzll(ull x) mr_noexcept {
		mr_assert(x);
		return __builtin_clzll(x);
	}

	MR_NODISCARD inline ll ctzll(ull x) mr_noexcept {
		mr_assert(x);
		return __builtin_ctzll(x);
	}

	MR_NODISCARD inline ll ffsll(ull x) mr_noexcept{
		return __builtin_ffsll(x);
	}

	MR_NODISCARD inline ll popcntll(ull x) mr_noexcept {
		return __builtin_popcountll(x);
	}

	MR_NODISCARD inline ll get_bit(ll src, ull x) mr_noexcept {
		return (src >> x) & 1;
	}

	template <bool dst>
	MR_NODISCARD inline bool compare_bit(ll src, ull x) mr_noexcept {
		return get_bit(src, x) == dst;
	}

	template <bool dst>
	MR_NODISCARD inline ll set_bit(ll src, ull x) mr_noexcept {
		if constexpr (dst)
			return src | (1ll << x);
		else
			return src & ~(1ll << x);
	}

	MR_NODISCARD inline ll lowbit(ll k) mr_noexcept {
		return k & -k;
	}
} // namespace mirai