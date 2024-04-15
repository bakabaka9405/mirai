#pragma once
#include <mirai/pch.hpp>
#include <mirai/math/vec2.hpp>
MR_NAMESPACE_BEGIN
MR_NODISCARD constexpr vec2f centroid(const vec2f& a, const vec2f& b, const vec2f& c) mr_noexcept {
	return (a + b + c) / 3.0;
}
MR_NODISCARD constexpr vec2f circumcenter(const vec2f& a, const vec2f& b, const vec2f& c) mr_noexcept {
	auto&& [x1, y1] = a;
	auto&& [x2, y2] = b;
	auto&& [x3, y3] = c;
	auto d1 = a.norm_square(), d2 = b.norm_square(), d3 = c.norm_square();
	auto e = 2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
	return { (d1 * (y2 - y3) + d2 * (y3 - y1) + d3 * (y1 - y2)) / e, (d1 * (x3 - x2) + d2 * (x1 - x3) + d3 * (x2 - x1)) / e };
}
MR_NAMESPACE_END