#pragma once
#include <mirai/pch.hpp>
#include <mirai/math/math_theory_basic.hpp>
MR_NAMESPACE_BEGIN
template <ull P>
struct modint {
	static_assert(P > 0, "P must be positive");
	ll val;
	constexpr modint(ll v = 0) mr_noexcept // NOLINT
		: val(((v % P) + P) % P) {}
	modint& operator=(ll v) mr_noexcept {
		val = ((v % P) + P) % P;
		return *this;
	}
	modint inv() const mr_noexcept {
		return mirai::inv(val, P);
	}
	modint operator+(const modint& v) const mr_noexcept { return modint{ val + v.val }; }
	modint operator-(const modint& v) const mr_noexcept { return modint{ val - v.val }; }
	modint operator*(const modint& v) const mr_noexcept { return modint{ val * v.val }; }
	modint operator/(const modint& v) const mr_noexcept { return *this * v.inv(); }
	modint& operator+=(ll v) mr_noexcept {
		val = (val + v) % P;
		return *this;
	}
	modint& operator-=(ll v) mr_noexcept {
		val = ((val - v) % P + P) % P;
		return *this;
	}
	modint& operator*=(ll v) mr_noexcept {
		val = val * v % P;
		return *this;
	}
	modint& operator/=(ll v) mr_noexcept {
		return *this = *this * modint{ v }.inv();
	}
};
MR_NAMESPACE_END