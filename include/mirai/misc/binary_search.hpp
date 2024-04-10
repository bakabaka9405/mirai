#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
enum bsearch_mode {
	sinistral, // mid 成立时把 l 向右偏
	dextral	   // mid 成立时把 r 向左偏
};
template <bsearch_mode mode>
ll int_bsearch(ll l, ll r, ll default_ans, auto&& check) {
	ll ans = default_ans, mid;
	while (l <= r) {
		if (mid = (l + r) / 2; check(mid)) {
			ans = mid;
			if (mode == sinistral)
				r = mid - 1;
			else
				l = mid + 1;
		}
		else {
			if (mode == sinistral)
				l = mid + 1;
			else
				r = mid - 1;
		}
	}
	return ans;
}

template <bsearch_mode mode>
double float_bsearch_by_eps(double l, double r, double eps, auto&& check) {
	while (r - l > eps) {
		if (double mid = (r + l) / 2; check(mid)) {
			if constexpr (mode == sinistral)
				r = mid;
			else
				l = mid;
		}
		else {
			if constexpr (mode == sinistral)
				l = mid;
			else
				r = mid;
		}
	}
	if constexpr (mode == sinistral)
		return l;
	else
		return r;
}

template <bsearch_mode mode>
double float_bsearch_by_times(double l, double r, size_t count, auto&& check) {
	while (count--) {
		if (double mid = (r + l) / 2; check(mid)) {
			if constexpr (mode == sinistral)
				r = mid;
			else
				l = mid;
		}
		else {
			if constexpr (mode == sinistral)
				l = mid;
			else
				r = mid;
		}
	}
	if constexpr (mode == sinistral)
		return l;
	else
		return r;
}
MR_NAMESPACE_END