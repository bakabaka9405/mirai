#pragma once
#include <mirai/pch.hpp>
#include <mirai/math/vec2.hpp>
#include <mirai/util/pipe.hpp>
#include <mirai/util/concepts.hpp>
#include <mirai/util/debug.hpp>
namespace mirai {
	template <typename Container>
		requires vec2_like<std::decay_t<decltype(*mr_begin(std::declval<Container>()))>>
	vector<vec2f> graham(Container&& container) mr_noexcept {
		vector<pair<vec2f, double>> buffer;
		vec2f p0 = *ranges::max_element(container, [&](auto&& lhs, auto&& rhs) { return rhs.y < lhs.y || (rhs.y == lhs.y && rhs.x < lhs.x); });
		for (auto&& vec : container | filter(not_equal_to(p0))) buffer.emplace_back(vec, (vec - p0).atan());
		ranges::sort(buffer, [&](auto&& lhs, auto&& rhs) {
			if (lhs.second != rhs.second) return lhs.second < rhs.second;
			return p0.distance_to(lhs.first) < p0.distance_to(rhs.first);
		});
		vector<vec2f> res;
		res.reserve(32);
		res.push_back(p0);
		size_t sz = 1;
		for (auto&& [vec, _] : buffer) {
			while (sz >= 2 && cross_product(res[sz - 1] - res[sz - 2], vec - res[sz - 1]) <= 0) {
				sz--;
				res.pop_back();
			}
			sz++;
			res.push_back(vec);
		}
		return res;
	}

	bool point_in_hull(const vector<vec2f>& h, const vec2f& a) mr_noexcept {
		ll n = (ll)h.size();
		if (cross_product(a - h[0], h[1] - h[0]) > 0 || cross_product(h[n - 1] - h[0], a - h[0]) > 0) return false;
		ll l = 1, r = n - 2;
		while (l < r) {
			ll mid = (l + r + 1) / 2;
			if (cross_product(h[mid] - h[0], a - h[0]) >= 0) l = mid;
			else r = mid - 1;
		}
		if (l >= n - 1 || l <= 0) return false;
		return cross_product((h[l + 1] - h[l]), (a - h[l])) >= 0;
	}

} // namespace mirai