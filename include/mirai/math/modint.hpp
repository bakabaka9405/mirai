#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
struct modint {
	ll val, P;
	explicit constexpr modint(ll p) mr_noexcept
		: val(0),
		  P(p) {}
	constexpr modint(ll v, ll p) mr_noexcept
		: val(v % p),
		  P(p) {}
	modint& operator=(ll v) mr_noexcept {
		val = v % P;
		return *this;
	}
	modint& operator+=(ll v) mr_noexcept {
		val = (val + v) % P;
		return *this;
	}
	modint& operator*=(ll v) mr_noexcept {
		val = val * v % P;
		return *this;
	}
};
MR_NAMESPACE_END