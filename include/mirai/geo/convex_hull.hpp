#pragma once
#include <mirai/pch.hpp>
#include <mirai/math/vector.hpp>
#include <mirai/util/pipeline.hpp>
namespace mirai {
	template <typename Container>
		requires vec2_like<std::decay_t<decltype(*mr_begin(std::declval<Container>()))>>
	vector<vec2f> graham(Container&& container) {
		vector<pair<vec2f, double>> buffer;
		vec2f p0 = *ranges::max_element(container, [&](auto&& lhs, auto&& rhs) { return rhs.y < lhs.y || (rhs.y == lhs.y && rhs.x < lhs.x); });
		for (auto&& vec : container | filter([&](auto&& vec) { return vec != p0; }))
			buffer.emplace_back(vec, (vec - p0).atan2());
		ranges::sort(buffer, [&](auto&& lhs, auto&& rhs) {
			if (lhs.second != rhs.second) return lhs.second < rhs.second;
			return p0.distance_to(lhs.first) < p0.distance_to(rhs.first);
		});
		vector<vec2f> res;
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

	double convex_hull_perimeter(vector<vec2f> hull) {
		double res = 0;
		for (size_t i = 0; i < hull.size(); i++)
			res += hull[i].distance_to(hull[(i + 1) % hull.size()]);
		return res;
	}

	double convex_hull_area(vector<vec2f> hull) {
		double res = 0;
		for (size_t i = 0; i < hull.size(); i++)
			res += cross_product(hull[i], hull[(i + 1) % hull.size()]);
		return res / 2;
	}
	
} // namespace mirai