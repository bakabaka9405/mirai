#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
template <typename T>
struct vec2 {
	T x, y;
	constexpr vec2() mr_noexcept = default;
	constexpr vec2(const vec2&) mr_noexcept = default;
	constexpr vec2(vec2&&) mr_noexcept = default;
	constexpr vec2& operator=(const vec2&) mr_noexcept = default;
	constexpr vec2(auto&& _x, auto&& _y) mr_noexcept : x(_x), y(_y) {}
	template <typename Y>
	constexpr explicit operator vec2<Y>() const mr_noexcept {
		return { static_cast<Y>(x), static_cast<Y>(y) };
	}
	constexpr vec2 operator+(const vec2& rt) const mr_noexcept { return { x + rt.x, y + rt.y }; }
	constexpr vec2 operator-(const vec2& rt) const mr_noexcept { return { x - rt.x, y - rt.y }; }
	constexpr vec2 operator*(auto&& c) const mr_noexcept { return { x * c, y * c }; }
	friend constexpr vec2 operator*(auto&& c, const vec2& vec) mr_noexcept { return vec * c; }
	constexpr vec2 operator/(auto&& c) const mr_noexcept { return { x / c, y / c }; }
	constexpr vec2& operator+=(const vec2& rt) mr_noexcept {
		x += rt.x, y += rt.y;
		return *this;
	}
	constexpr vec2& operator-=(const vec2& rt) mr_noexcept {
		x -= rt.x, y -= rt.y;
		return *this;
	}
	constexpr vec2& operator*=(auto&& c) mr_noexcept {
		x *= c, y *= c;
		return *this;
	}
	constexpr vec2& operator/=(auto&& c) mr_noexcept {
		x /= c, y /= c;
		return *this;
	}
	MR_NODISCARD constexpr auto norm() const mr_noexcept { return std::sqrt(x * x + y * y); }
	constexpr bool operator==(const vec2& rt) const mr_noexcept { return std::equal_to<T>()(x, rt.x) && std::equal_to<T>()(y, rt.y); }
	constexpr bool operator!=(const vec2& rt) const mr_noexcept { return !this->operator==(rt); }
	friend std::ostream& operator<<(std::ostream& out, const vec2& vec) mr_noexcept {
		return out << vec.to_string();
	}
	MR_NODISCARD constexpr string to_string() const mr_noexcept {
		return "(" + std::to_string(x) + "," + std::to_string(y) + ")";
	}
	MR_NODISCARD friend constexpr vec2 lerp(const vec2& lhs, const vec2& rhs, double t) mr_noexcept {
		return lhs + t * (rhs - lhs);
	}
	MR_NODISCARD friend constexpr auto dot_product(const vec2& lhs, const vec2& rhs) mr_noexcept { return lhs.x * rhs.x + lhs.y * rhs.y; }
	MR_NODISCARD friend constexpr auto dot_product_abs(const vec2& lhs, const vec2& rhs) mr_noexcept { return std::abs(dot_product(lhs, rhs)); }
	MR_NODISCARD friend constexpr auto dot_product(const vec2& A, const vec2& B, const vec2& C) mr_noexcept { return dot_product(B - A, C - A); }
	MR_NODISCARD friend constexpr auto dot_product_abs(const vec2& A, const vec2& B, const vec2& C) mr_noexcept { return std::abs(dot_product(A, B, C)); }
	MR_NODISCARD friend constexpr auto cross_product(const vec2& lhs, const vec2& rhs) mr_noexcept { return lhs.x * rhs.y - lhs.y * rhs.x; }
	MR_NODISCARD constexpr auto atan() const mr_noexcept { return std::atan2(y, x); }
	MR_NODISCARD constexpr auto distance_to(const vec2& rhs) const mr_noexcept {
		return std::sqrt((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y));
	}
	MR_NODISCARD constexpr auto distance_to_square(const vec2& rhs) const mr_noexcept {
		return (x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y);
	}
	MR_NODISCARD friend constexpr auto manhattan_distance_to(const vec2& lhs, const vec2& rhs) mr_noexcept {
		return std::abs(lhs.x - rhs.x) + std::abs(lhs.y - rhs.y);
	}
	MR_NODISCARD constexpr vec2 project_to(const vec2& dst) const mr_noexcept {
		return dst * dot_product(*this, dst) / dot_product(dst, dst);
	}
	MR_NODISCARD friend constexpr vec2 projection(const vec2& src, const vec2& dst) {
		return src.project_to(dst);
	}
	void rotate(double radian) mr_noexcept {
		std::tie(x, y) = std::make_pair(x * std::cos(radian) - y * std::sin(radian), x * std::sin(radian) + y * std::cos(radian));
	}
	MR_NODISCARD constexpr vec2 rotated(double radian) const mr_noexcept {
		return { x * std::cos(radian) - y * std::sin(radian), x * std::sin(radian) + y * std::cos(radian) };
	}
};
using vec2i = vec2<ll>;
using vec2f = vec2<double>;

template <typename T>
constexpr T interpolation(const T& lt, const T& rt, const double p) {
	return lt * p + rt * (1 - p);
}

template <typename T>
string to_string(const vec2<T>& vec) {
	return vec.to_string();
}

MR_NAMESPACE_END