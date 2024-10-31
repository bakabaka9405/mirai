#pragma once
#include <mirai/pch.hpp>
#include <mirai/math/vec2.hpp>
MR_NAMESPACE_BEGIN
struct circle {
	vec2f center;
	double radius;
	constexpr circle() mr_noexcept = default;
	constexpr circle(const circle&) mr_noexcept = default;
	constexpr circle(circle&&) mr_noexcept = default;
	constexpr circle& operator=(const circle&) mr_noexcept = default;
	constexpr circle(vec2f _center, double _radius) mr_noexcept : center(_center), radius(_radius) {}
	constexpr bool operator==(const circle& rt) const mr_noexcept { return center == rt.center && std::equal_to<>()(radius, rt.radius); }
	constexpr bool operator!=(const circle& rt) const mr_noexcept { return !this->operator==(rt); }
	MR_NODISCARD constexpr static circle from_diameter(const vec2f& a, const vec2f& b) mr_noexcept {
		return { (a + b) / 2, a.distance_to(b) / 2 };
	}
	MR_NODISCARD constexpr static circle from_triangle(const vec2f& a, const vec2f& b, const vec2f& c) mr_noexcept {
		auto&& [x1, y1] = a;
		auto&& [x2, y2] = b;
		auto&& [x3, y3] = c;
		auto d1 = a.norm_square(), d2 = b.norm_square(), d3 = c.norm_square();
		auto e = 2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
		vec2f o{ (d1 * (y2 - y3) + d2 * (y3 - y1) + d3 * (y1 - y2)) / e,
				 (d1 * (x3 - x2) + d2 * (x1 - x3) + d3 * (x2 - x1)) / e };
		return { o, o.distance_to(a) };
	}
	MR_NODISCARD constexpr bool contains(const vec2f& p) const mr_noexcept { return center.distance_to(p) <= radius; }
};

MR_NAMESPACE_END