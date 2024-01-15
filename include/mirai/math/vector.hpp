#pragma once
#include <mirai/pch.hpp>
namespace mirai {
	template <typename T>
	struct vec2 {
		T x, y;
		constexpr vec2(auto&& _x, auto&& _y) mr_noexcept : x(_x), y(_y) {}
		template <typename Y>
		constexpr operator vec2<Y>() const mr_noexcept {
			return { static_cast<Y>(x), static_cast<Y>(y) };
		}
		constexpr vec2 operator+(const vec2& rt) const mr_noexcept { return { x + rt.x, y + rt.y }; }
		constexpr vec2 operator-(const vec2& rt) const mr_noexcept { return { x - rt.x, y - rt.y }; }
		constexpr vec2 operator*(auto&& c) const mr_noexcept { return { x * c, y * c }; }
		friend vec2 operator*(auto&& c, const vec2& vec) mr_noexcept { return vec * c; }
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
		constexpr double norm() const mr_noexcept { return sqrt(x * x + y * y); }
		constexpr bool operator==(const vec2& rt) const mr_noexcept { return std::equal_to<T>()(x, rt.x) && std::equal_to<T>()(y, rt.y); }
		constexpr bool operator!=(const vec2& rt) const mr_noexcept { return !this->operator==(rt); }
		friend std::ostream& operator<<(std::ostream& out, const vec2& vec) mr_noexcept {
			return out << format("({0},{1})", vec.x, vec.y);
		}
		inline string to_string() const mr_noexcept {
			return format("({0},{1})", x, y);
		}
		MR_NODISCARD friend inline vec2 lerp(const vec2& lhs, const vec2& rhs, double t) mr_noexcept {
			return lhs + t * (rhs - lhs);
		}
	};
	using vec2i = vec2<ll>;
	using vec2f = vec2<double>;
	template <typename T>
	struct vec3 {
		T x, y, z;
		constexpr vec3(auto _x, auto _y, auto _z) mr_noexcept : x(_x), y(_y), z(_z) {}
		template <typename Y>
		constexpr operator vec3<Y>() const mr_noexcept {
			return { static_cast<Y>(x), static_cast<Y>(y), static_cast<Y>(z) };
		}
		constexpr vec3 operator+(const vec3& rt) const mr_noexcept { return { x + rt.x, y + rt.y, z + rt.z }; }
		constexpr vec3 operator-(const vec3& rt) const mr_noexcept { return { x - rt.x, y - rt.y, z - rt.z }; }
		constexpr vec3 operator*(auto&& c) const mr_noexcept { return { x * c, y * c, z * c }; }
		friend vec3 operator*(auto&& c, const vec3& vec) mr_noexcept { return vec * c; }
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
		constexpr double norm() const mr_noexcept { return sqrt(x * x + y * y + z * z); }
		friend std::ostream& operator<<(std::ostream& out, const vec3& vec) mr_noexcept {
			return out << format("({0},{1},{2})", vec.x, vec.y, vec.z);
		}
		string to_string() const mr_noexcept {
			return format("({0},{1},{2})", x, y, z);
		}
		MR_NODISCARD friend inline vec3 lerp(const vec3& lhs, const vec3& rhs, double t) mr_noexcept {
			return lhs + t * (rhs - lhs);
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
} // namespace mirai