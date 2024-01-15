#pragma once
#include <mirai/pch.hpp>
namespace mirai {
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
			if (k & 1) res = res * x % P;
			x = x * x % P;
			k >>= 1;
		}
		return res;
	}

	MR_NODISCARD inline ull inv(ull x, ull P) mr_noexcept {
		return fastpow(x, P - 2, P);
	}

	inline void exgcd(ll a, ll b, ll& x, ll& y) mr_noexcept {
		if (b == 0) {
			x = 1, y = 0;
			return;
		}
		exgcd(b, a % b, y, x);
		y -= a / b * x;
	}

	inline void generate_inv_arr(std::random_access_iterator auto it, size_t n, size_t P) mr_noexcept {
		*(it + 1) = 1;
		auto p = it + 2;
		for (size_t i = 2; i <= n; ++i, ++p) {
			*p = (P - P / i) * it[P % i] % P;
		}
	}
} // namespace mirai