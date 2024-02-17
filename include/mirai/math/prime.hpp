#pragma once
#include <mirai/pch.hpp>
#include <mirai/math/math_theory_basic.hpp>
MR_NAMESPACE_BEGIN
MR_NODISCARD inline bool miller_rabin(ll n) mr_noexcept {
	if (n < 3 || n % 2 == 0) return n == 2;
	ll u = n - 1, t = 0;
	while (u % 2 == 0) u /= 2, ++t;
	constexpr ll test_time = 8;
	for (ll i = 0; i < test_time; ++i) {
		ll a = rand() % (n - 2) + 2, v = fastpow(a, u, n);
		if (v == 1) continue;
		ll s;
		for (s = 0; s < t; ++s) {
			if (v == n - 1) break;
			v = v * v % n;
		}
		if (s == t) return 0;
	}
	return 1;
}

MR_NAMESPACE_END