#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
struct modint {
	ll val, P;
	modint(ll p)
		: val(0), P(p) {}
	modint(ll v, ll p)
		: val(v % p), P(p) {}
	modint& operator=(ll v) {
		val = v % P;
		return *this;
	}
	modint& operator+=(ll v) {
		val = (val + v) % P;
		return *this;
	}
	modint& operator*=(ll v) {
		val = val * v % P;
		return *this;
	}
};
MR_NAMESPACE_END