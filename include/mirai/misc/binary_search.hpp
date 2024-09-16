#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
enum bsearch_mode {
	left,	// 答案越小越好
	right	// 答案越大越好
};
template <bsearch_mode mode>
ll int_bsearch(ll l, ll r, ll default_ans, auto&& check) {
	ll ans = default_ans, mid;
	while (l <= r) {
		if (mid = (l + r) / 2; check(mid)) {
			ans = mid;
			if constexpr (mode == left)
				r = mid - 1;
			else
				l = mid + 1;
		}
		else {
			if constexpr (mode == left)
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
			if constexpr (mode == left)
				r = mid;
			else
				l = mid;
		}
		else {
			if constexpr (mode == left)
				l = mid;
			else
				r = mid;
		}
	}
	if constexpr (mode == left)
		return l;
	else
		return r;
}

template <bsearch_mode mode>
double float_bsearch_by_times(double l, double r, size_t count, auto&& check) {
	while (count--) {
		if (double mid = (r + l) / 2; check(mid)) {
			if constexpr (mode == left)
				r = mid;
			else
				l = mid;
		}
		else {
			if constexpr (mode == left)
				l = mid;
			else
				r = mid;
		}
	}
	if constexpr (mode == left)
		return l;
	else
		return r;
}
MR_NAMESPACE_END