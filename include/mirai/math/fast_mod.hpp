#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
struct fastmod {
	i64 f, l;
	u64 m, d;
	explicit fastmod(ull d)
		: d(d) {
		l = 64 - __builtin_clzll(d - 1);
		constexpr i128 one = 1;
		i128 M = ((one << (64 + l)) + (one << l)) / d;
		if (M < (one << 64))
			f = 1, m = M;
		else
			f = 0, m = M - (one << 64);
	}
	friend i128 operator/(i128 n, const fastmod& mod_num) { // get n / d
		if (mod_num.f)
			return u128(n) * mod_num.m >> 64u >> mod_num.l;
		else {
			u64 t = u128(n) * mod_num.m >> 64u;
			return (((n - t) >> 1u) + t) >> (mod_num.l - 1);
		}
	}
	friend u64 operator%(u64 n, const fastmod& mod_num) { // get n % d
		return n - n / mod_num * mod_num.d;
	}
};
MR_NAMESPACE_END