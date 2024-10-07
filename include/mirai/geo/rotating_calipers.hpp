#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/coro.hpp>
#include <mirai/math/vec2.hpp>
#include <mirai/util/pipe.hpp>
#include <mirai/geo/polygon.hpp>
MR_NAMESPACE_BEGIN
generator<tuple<vec2f, vec2f, vec2f>> rotating_parallel_coro(const vector<vec2f>& hull) {
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

generator<tuple<vec2f, vec2f, vec2f, vec2f, vec2f>> rotating_rectangle_coro(const vector<vec2f>& hull) {
	if (hull.size() < 3) co_return;
	ull m = hull.size();
	ull A = 0, B = 1, C = 2, D = 1, E = 1;
	auto next = [&](ull x) mr_noexcept { return (x + 1) % m; };
	for (; A < m; A++, B = next(B)) {
		while (dot_product(hull[A], hull[B], hull[D]) < dot_product(hull[A], hull[B], hull[next(D)])) D = next(D);
		while (polygon_area({ hull[A], hull[B], hull[E] }) < polygon_area({ hull[A], hull[B], hull[next(E)] })) E = next(E);
		if (C < E) C = E;
		while (dot_product(hull[B], hull[A], hull[C]) < dot_product(hull[B], hull[A], hull[next(C)])) C = next(C);
		co_yield { hull[A], hull[B], hull[C], hull[D], hull[E] };
	}
}
MR_NAMESPACE_END