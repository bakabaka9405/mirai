#pragma once
#include <mirai/pch.hpp>
#include <mirai/math/vec2.hpp>
MR_NAMESPACE_BEGIN
struct segment {
	vec2f a, b;
	constexpr segment() mr_noexcept = default;
	constexpr segment(const segment&) mr_noexcept = default;
	constexpr segment(segment&&) mr_noexcept = default;
	constexpr segment& operator=(const segment&) mr_noexcept = default;
	constexpr segment(vec2f a, vec2f b) mr_noexcept
		: a(a),
		  b(b) {}
	constexpr segment(double x1, double y1, double x2, double y2) mr_noexcept
		: a(x1, y1),
		  b(x2, y2) {}
	MR_NODISCARD constexpr bool operator==(const segment& other) const mr_noexcept { return a == other.a && b == other.b; }
	MR_NODISCARD constexpr bool operator!=(const segment& other) const mr_noexcept { return !(*this == other); }
	MR_NODISCARD constexpr auto length() const mr_noexcept {
		return a.distance_to(b);
	}
	MR_NODISCARD constexpr double atan() const mr_noexcept {
		return (b - a).atan();
	}
	MR_NODISCARD constexpr auto distance_to(const vec2f& p) const mr_noexcept {
		auto ap = p - a;
		auto ab = b - a;
		auto u = dot_product(ap, ab) / ab.norm();
		if (u < 0) return a.distance_to(p);
		if (u > 1) return b.distance_to(p);
		return a.distance_to(a + ab * u);
	}
	MR_NODISCARD constexpr vec2f lerp(double t) const mr_noexcept {
		return a + t * (b - a);
	}
};

MR_NODISCARD constexpr bool bounding_box_test(const segment& s1, const segment& s2) mr_noexcept {
	vec2f left_top_1{ std::min(s1.a.x, s1.b.x), std::max(s1.a.y, s1.b.y) }, right_bottom_1{ std::max(s1.a.x, s1.b.x), std::min(s1.a.y, s1.b.y) };
	vec2f left_top_2{ std::min(s2.a.x, s2.b.x), std::max(s2.a.y, s2.b.y) }, right_bottom_2{ std::max(s2.a.x, s2.b.x), std::min(s2.a.y, s2.b.y) };
	return !(left_top_1.x > right_bottom_2.x || left_top_2.x > right_bottom_1.x || left_top_1.y < right_bottom_2.y || left_top_2.y < right_bottom_1.y);
}

MR_NODISCARD constexpr bool straddle_test(const segment& s1, const segment& s2) mr_noexcept {
	auto d1 = cross_product(s2.a - s1.a, s1.b - s1.a);
	auto d2 = cross_product(s2.b - s1.a, s1.b - s1.a);
	auto d3 = cross_product(s1.a - s2.a, s2.b - s2.a);
	auto d4 = cross_product(s1.b - s2.a, s2.b - s2.a);
	return d1 * d2 < 0 && d3 * d4 < 0;
}

MR_NODISCARD constexpr bool intersect_test(const segment& s1, const segment& s2) mr_noexcept {
	return bounding_box_test(s1, s2) && straddle_test(s1, s2);
}

MR_NODISCARD constexpr vec2f intersection(const segment& s1, const segment& s2) mr_noexcept {
	auto u = (double)cross_product(s2.b - s2.a, s1.a - s2.a) / cross_product(s2.b - s2.a, s1.b - s1.a);
	return s1.a + (s1.b - s1.a) * u;
}

MR_NODISCARD constexpr std::optional<vec2f> intersection_opt(const segment& s1, const segment& s2) mr_noexcept {
	if (!intersect_test(s1, s2)) return std::nullopt;
	return intersection(s1, s2);
}
MR_NAMESPACE_END