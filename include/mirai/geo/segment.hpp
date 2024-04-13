#pragma once
#include <mirai/pch.hpp>
#include <mirai/math/vec2.hpp>
MR_NAMESPACE_BEGIN
template <typename T>
struct segment {
	vec2<T> a, b;
	constexpr segment(vec2<T> a, vec2<T> b) mr_noexcept
		: a(std::move(a)),
		  b(std::move(b)) {}
	constexpr segment(T x1, T y1, T x2, T y2) mr_noexcept
		: a(x1, y1),
		  b(x2, y2) {}
	MR_NODISCARD constexpr T length() const mr_noexcept {
		return a.distance_to(b);
	}
	MR_NODISCARD constexpr double atan() const mr_noexcept {
		return (b - a).atan();
	}
	MR_NODISCARD constexpr auto distance_to(const vec2<T>& p) const mr_noexcept {
		auto ap = p - a;
		auto ab = b - a;
		auto u = dot_product(ap, ab) / ab.norm();
		if (u < 0) return a.distance_to(p);
		if (u > 1) return b.distance_to(p);
		return a.distance_to(a + ab * u);
	}
	MR_NODISCARD constexpr vec2<T> lerp(double t) const mr_noexcept {
		return a + t * (b - a);
	}
};

using segmenti = segment<ll>;
using segmentf = segment<double>;
MR_NAMESPACE_END