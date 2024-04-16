#pragma once
#include <mirai/pch.hpp>
#include <mirai/math/vec2.hpp>
#include <mirai/util/pipe.hpp>
#include <mirai/util/concepts.hpp>
#include <mirai/geo/polar_angle_sort.hpp>
namespace mirai {
	vector<vec2f> graham(vector<vec2f>& container) mr_noexcept {
		for (auto& vec : container)
			if (vec.y < container[0].y || (vec.y == container[0].y && vec.x < container[0].x)) swap(vec, container[0]);
		polar_angle_sorting(container.begin() + 1, container.end(), container[0]);
		vector<vec2f> res;
		res.reserve(32);
		res.push_back(container[0]);
		size_t sz = 1;
		for (auto&& vec : container) {
			while (sz >= 2 && cross_product(res[sz - 1] - res[sz - 2], vec - res[sz - 1]) <= 0) {
				sz--;
				res.pop_back();
			}
			sz++;
			res.push_back(vec);
		}
		return res;
	}

	auto graham(vector<vec2f>&& container) mr_noexcept {
		return graham(container);
	}

	auto graham(range auto&& rg) mr_noexcept {
		vector<vec2f> container(mr_begin(rg), mr_end(rg));
		return graham(container);
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