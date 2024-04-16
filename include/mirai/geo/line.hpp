#include <mirai/pch.hpp>
#include <mirai/math/vec2.hpp>
MR_NAMESPACE_BEGIN
struct line {
	vec2f o;
	vec2f d;
	constexpr line() mr_noexcept = default;
	constexpr line(const line&) mr_noexcept = default;
	constexpr line(line&&) mr_noexcept = default;
	constexpr line& operator=(const line&) mr_noexcept = default;
	constexpr line(vec2f o, vec2f d) mr_noexcept
		: o(o),
		  d(d) {}
	MR_NODISCARD static constexpr line from_segment(const vec2f& p1, const vec2f& p2) mr_noexcept { return { p1, p2 - p1 }; }
	MR_NODISCARD constexpr bool operator==(const line& other) const mr_noexcept { return o == other.o && d == other.d; }
	MR_NODISCARD constexpr bool operator!=(const line& other) const mr_noexcept { return !(*this == other); }
	MR_NODISCARD constexpr auto distance_to(const vec2f& p) const mr_noexcept {
		return cross_product_abs(d, p - o) / d.norm();
	}
	MR_NODISCARD constexpr bool is_parallel_to(const line& other) const mr_noexcept {
		return cross_product_abs(d, other.d) < 1e-9;
	}
	MR_NODISCARD constexpr bool is_perpendicular_to(const line& other) const mr_noexcept {
		return dot_product_abs(d, other.d) < 1e-9;
	}
	MR_NODISCARD constexpr bool contains(const vec2f& p) const mr_noexcept {
		return cross_product_abs(d, p - o) < 1e-9;
	}
	MR_NODISCARD constexpr bool intersects_with(const line& other) const mr_noexcept {
		return !is_parallel_to(other);
	}
	MR_NODISCARD constexpr friend bool intersect_test(const line& l1, const line& l2) mr_noexcept {
		return !l1.is_parallel_to(l2);
	}
	MR_NODISCARD constexpr friend vec2f intersection(const line& l1, const line& l2) mr_noexcept {
		auto u = cross_product(l2.d, l1.o - l2.o) / cross_product(l2.d, l1.d);
		return l1.o + l1.d * u;
	}
	MR_NODISCARD constexpr friend std::optional<vec2f> intersection_opt(const line& l1, const line& l2) mr_noexcept {
		if (l1.is_parallel_to(l2)) return std::nullopt;
		auto u = cross_product(l2.d, l1.o - l2.o) / cross_product(l2.d, l1.d);
		return l1.o + l1.d * u;
	}
};
MR_NAMESPACE_END