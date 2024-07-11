#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
template <typename T>
struct vec3 {
	T x, y, z;
	constexpr vec3(auto _x, auto _y, auto _z) mr_noexcept : x(_x), y(_y), z(_z) {}
	template <typename Y>
	constexpr explicit operator vec3<Y>() const mr_noexcept {
		return { static_cast<Y>(x), static_cast<Y>(y), static_cast<Y>(z) };
	}
	constexpr vec3 operator+(const vec3& rt) const mr_noexcept { return { x + rt.x, y + rt.y, z + rt.z }; }
	constexpr vec3 operator-(const vec3& rt) const mr_noexcept { return { x - rt.x, y - rt.y, z - rt.z }; }
	constexpr vec3 operator*(auto&& c) const mr_noexcept { return { x * c, y * c, z * c }; }
	friend constexpr vec3 operator*(auto&& c, const vec3& vec) mr_noexcept { return vec * c; }
	constexpr vec3 operator/(auto&& c) const mr_noexcept { return { x / c, y / c, z / c }; }
	constexpr vec3& operator+=(const vec3& rt) mr_noexcept {
		x += rt.x, y += rt.y, z += rt.z;
		return *this;
	}
	constexpr vec3& operator-=(const vec3& rt) mr_noexcept {
		x -= rt.x, y -= rt.y, z -= rt.z;
		return *this;
	}
	constexpr vec3& operator*=(auto&& c) mr_noexcept {
		x *= c, y *= c, z *= c;
		return *this;
	}
	constexpr vec3& operator/=(auto&& c) mr_noexcept {
		x /= c, y /= c, z /= c;
		return *this;
	}
	constexpr bool operator==(const vec3& rt) const mr_noexcept {
		return std::equal_to<T>()(x, rt.x) && std::equal_to<T>()(y, rt.y) && std::equal_to<T>()(z, rt.z);
	}
	constexpr bool operator!=(const vec3& rt) const mr_noexcept { return !this->operator==(rt); }
	MR_NODISCARD constexpr auto norm_square() const mr_noexcept { return x * x + y * y + z * z; }
	MR_NODISCARD constexpr auto norm() const mr_noexcept { return std::sqrt(x * x + y * y + z * z); }
	MR_NODISCARD constexpr auto normalized() const mr_noexcept { return *this / norm(); }
	friend std::ostream& operator<<(std::ostream& out, const vec3& vec) mr_noexcept {
		return out << vec.to_string();
	}
	MR_NODISCARD constexpr string to_string() const mr_noexcept {
		return "(" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + ")";
	}
	MR_NODISCARD friend inline vec3 lerp(const vec3& lhs, const vec3& rhs, double t) mr_noexcept {
		return lhs + t * (rhs - lhs);
	}
	MR_NODISCARD constexpr auto distance_to(const vec3& rhs) const mr_noexcept {
		return std::sqrt((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y) + (z - rhs.z) * (z - rhs.z));
	}
	MR_NODISCARD friend constexpr auto manhattan_distance_to(const vec3& lhs, const vec3& rhs) mr_noexcept {
		return std::abs(lhs.x - rhs.x) + std::abs(lhs.y - rhs.y) + std::abs(lhs.z - rhs.z);
	}
	MR_NODISCARD constexpr auto dot(const vec3& rt) const mr_noexcept { return x * rt.x + y * rt.y + z * rt.z; }
};
using vec3i = vec3<ll>;
using vec3f = vec3<double>;

template <typename T>
string to_string(const vec3<T>& vec) {
	return vec.to_string();
}
MR_NAMESPACE_END
