#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
template <typename T>
struct vec2 {
	T x, y;
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
	MR_NODISCARD friend constexpr auto cross_product(const vec2& lhs, const vec2& rhs) mr_noexcept {
		return lhs.x * rhs.y - lhs.y * rhs.x;
	}
	MR_NODISCARD constexpr auto atan() const mr_noexcept { return std::atan2(y, x); }

	MR_NODISCARD constexpr auto distance_to(const vec2& rhs) const mr_noexcept {
		return std::sqrt((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y));
	}
	MR_NODISCARD friend constexpr auto manhattan_distance_to(const vec2& lhs, const vec2& rhs) mr_noexcept {
		return std::abs(lhs.x - rhs.x) + std::abs(lhs.y - rhs.y);
	}
	void rotate(double angle) mr_noexcept {
		std::tie(x, y) = std::make_pair(x * std::cos(angle) - y * std::sin(angle), x * std::sin(angle) + y * std::cos(angle));
	}
	MR_NODISCARD constexpr vec2 rotated(double angle) const mr_noexcept {
		return { x * std::cos(angle) - y * std::sin(angle), x * std::sin(angle) + y * std::cos(angle) };
	}
};
using vec2i = vec2<ll>;
using vec2f = vec2<double>;
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
	MR_NODISCARD constexpr auto norm() const mr_noexcept { return std::sqrt(x * x + y * y + z * z); }
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
};
using vec3i = vec3<ll>;
using vec3f = vec3<double>;

template <typename T>
constexpr T interpolation(const T& lt, const T& rt, const double p) {
	return lt * p + rt * (1 - p);
}

template <typename T>
string to_string(const vec2<T>& vec) {
	return vec.to_string();
}

template <typename T>
string to_string(const vec3<T>& vec) {
	return vec.to_string();
}
MR_NAMESPACE_END

namespace std {
	template <size_t N, typename T>
	constexpr decltype(auto) get(const mirai::vec2<T>& vec) mr_noexcept { // NOLINT
		if constexpr (N == 0)
			return vec.x;
		else if constexpr (N == 1)
			return vec.y;
		else {
			static_assert(N < 2, "index out of range");
		}
	}

	template <size_t N, typename T>
	T& get(mirai::vec2<T>& vec) mr_noexcept { // NOLINT
		if constexpr (N == 0)
			return vec.x;
		else if constexpr (N == 1)
			return vec.y;
		else {
			static_assert(N < 2, "index out of range");
		}
	}

	template <typename T>
	struct tuple_size<mirai::vec2<T>> : std::integral_constant<std::size_t, 2> {}; // NOLINT

	template <size_t N, typename T>
	struct tuple_element<N, mirai::vec2<T>> { // NOLINT
		using type = T;
	};
} // namespace std