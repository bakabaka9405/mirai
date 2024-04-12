#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/coro.hpp>
#include <mirai/math/vector.hpp>
#include <mirai/util/pipe.hpp>
#include <mirai/geo/polygon.hpp>
MR_NAMESPACE_BEGIN
generator<tuple<vec2f, vec2f, vec2f>> rotating_parallel(const vector<vec2f>& hull) {
	if (hull.size() < 2) co_return;
	ull m = hull.size();
	ull it = 0;
	for (auto&& [p, q] : zip(hull, hull | skip(1))) {
		while (polygon_area({ p, q, hull[it] }) < polygon_area({ p, q, hull[(it + 1) % m] })) it = (it + 1) % m;
		co_yield { p, q, hull[it] };
	}
	while (polygon_area({ hull.back(), hull.front(), hull[it] }) < polygon_area({ hull.back(), hull.front(), hull[(it + 1) % m] })) it = (it + 1) % m;
	co_yield { hull.back(), hull.front(), hull[it] };
}
MR_NAMESPACE_END