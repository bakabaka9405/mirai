#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/concepts.hpp>
#include <mirai/math/vec2.hpp>
MR_NAMESPACE_BEGIN
void polar_angle_sorting(std::random_access_iterator auto begin, std::random_access_iterator auto end, const vec2f& o) mr_noexcept {
	std::sort(begin, end, [&](const vec2f& a, const vec2f& b) {
		auto c = cross_product(a - o, b - o);
		if (c == 0) return a.distance_to(o) < b.distance_to(o);
		return c > 0;
	});
}
void polar_angle_sorting(range auto&& rg, const vec2f& o) mr_noexcept {
	polar_angle_sorting(mr_begin(rg), mr_end(rg), o);
}
MR_NAMESPACE_END