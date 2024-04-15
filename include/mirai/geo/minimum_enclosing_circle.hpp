#pragma once
#include <mirai/pch.hpp>
#include <mirai/math/vec2.hpp>
#include <mirai/util/pipe.hpp>
#include <mirai/geo/circle.hpp>
#include <mirai/util/random.hpp>
MR_NAMESPACE_BEGIN
circle minimum_enclosing_circle(vector<vec2f>& points) {
	shuffle(points);
	circle res{};
	for (auto i : views::iota(0ull, points.size())) {
		if (res.contains(points[i])) continue;
		res = { points[i], 0 };
		for (auto j : views::iota(0ull, i)) {
			if (res.contains(points[j])) continue;
			res = circle::from_diameter(points[i], points[j]);
			for (auto k : views::iota(0ull, j)) {
				if (res.contains(points[k])) continue;
				res = circle::from_triangle(points[i], points[j], points[k]);
			}
		}
	}
	return res;
}
MR_NAMESPACE_END