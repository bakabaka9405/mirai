#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
template <typename T>
struct IE {};

template <typename T>
inline constexpr T IE_v = IE<T>::value;

template <typename T>
	requires integral<T>
struct IE<T> {
	constexpr static T val = T(1);
};

MR_NODISCARD inline ull
fastpow(ull x, ull k, ull P) mr_noexcept {
	ull res = 1;
	while (k) {
		if (k & 1ull) res = res * x % P;
		x = x * x % P;
		k >>= 1ull;
	}
	return res;
}

MR_NODISCARD inline auto exgcd(ll a, ll b) mr_noexcept {
	ll x1 = 1, x2 = 0, x3 = 0, x4 = 1;
	while (b != 0) {
		ll c = a / b;
		std::tie(x1, x2, x3, x4, a, b) = std::make_tuple(x3, x4, x1 - x3 * c, x2 - x4 * c, b, a - b * c);
	}
	return std::make_tuple(a, x1, x2);
}

MR_NODISCARD inline ull inv(ull x, ull P) mr_noexcept {
	return std::get<1>(exgcd((ll)x, (ll)P));
}

MR_NAMESPACE_END