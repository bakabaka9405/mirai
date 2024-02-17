#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
MR_NODISCARD MR_API constexpr ull get_bit(ull src, ull x) mr_noexcept {
	MR_ASSUME(x <= 63);
	return (src >> x) & 1ull;
}

template <bool dst>
MR_NODISCARD MR_API constexpr bool compare_bit(ull src, ull x) mr_noexcept {
	MR_ASSUME(x <= 63);
	return get_bit(src, x) == dst;
}

template <bool dst>
MR_NODISCARD MR_API constexpr ull set_bit(ull src, ull x) mr_noexcept {
	MR_ASSUME(x <= 63);
	if constexpr (dst)
		return src | (1ull << x);
	else
		return src & ~(1ull << x);
}

MR_NODISCARD MR_API constexpr ull lowbit(ull k) mr_noexcept {
	return k & -k;
}
MR_NAMESPACE_END