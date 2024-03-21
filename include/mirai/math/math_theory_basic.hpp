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

MR_NODISCARD inline ull inv(ull x, ull P) mr_noexcept {
	return fastpow(x, P - 2, P);
}

MR_NODISCARD inline auto exgcd(ll a, ll b) mr_noexcept {
	ll x1 = 1, x2 = 0, x3 = 0, x4 = 1;
	while (b != 0) {
		ll c = a / b;
		std::tie(x1, x2, x3, x4, a, b) = std::make_tuple(x3, x4, x1 - x3 * c, x2 - x4 * c, b, a - b * c);
	}
	return std::make_tuple(a, x1, x2);
}

inline void generate_inv_arr(std::random_access_iterator auto it, size_t n, ull P) mr_noexcept {
	it[1] = 1;
	auto p = it + 2;
	for (size_t i = 2; i <= n; ++i, ++p) {
		*p = (P - P / i) * it[P % i] % P;
	}
}
MR_NAMESPACE_END