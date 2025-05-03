#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
namespace functor {
	template <typename T = void>
	struct max {
		constexpr T operator()(const T& a, const T& b) const {
			return std::max(a, b);
		}
	};

	template <>
	struct max<void> {
		constexpr auto operator()(auto&& a, auto&& b) const {
			return std::max(a, b);
		}
	};

	template <typename T = void>
	struct min {
		constexpr T operator()(const T& a, const T& b) const {
			return std::min(a, b);
		}
	};

	template <>
	struct min<void> {
		constexpr auto operator()(auto&& a, auto&& b) const {
			return std::min(a, b);
		}
	};

	struct gcd {
		constexpr auto operator()(std::integral auto a, std::integral auto b) const {
			return std::gcd(a, b);
		}
	};

	struct lcm {
		constexpr auto operator()(std::integral auto a, std::integral auto b) const {
			return std::lcm(a, b);
		}
	};

	struct add {
		constexpr auto operator()(auto&& a, auto&& b) const {
			return a + b;
		}
	};

	template <ull P>
	struct add_mod {
		constexpr auto operator()(ull a, ull b) const {
			return (a + b) % P;
		}
	};

	template <ull P>
	struct add_mod_save {
		constexpr auto operator()(ull a, ull b) const {
			return ((a + b) % P + P) % P;
		}
	};
} // namespace functor
MR_NAMESPACE_END