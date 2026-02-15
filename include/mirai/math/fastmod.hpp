#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
// int 范围内的模数可以把移位数改为 62
struct fastmod {
	ll m, p;
	void init(ll p_) {
		m = ll(((__int128)1 << 64) / p_);
		p = p_;
	}
	ll operator()(ll x) const noexcept {
		x -= (ll)((__int128(x) * m) >> 64) * p;
		if (x >= p) x -= p;
		return x;
	}
};
ll operator%(ll a, const fastmod& mod) {
	return mod(a);
}
ll& operator%=(ll& a, const fastmod& mod) {
	return a = mod(a);
}
MR_NAMESPACE_END