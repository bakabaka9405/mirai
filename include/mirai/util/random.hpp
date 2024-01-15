#pragma once
#include <mirai/pch.hpp>
#include <random>
namespace mirai {

	inline random_device __gen;

	inline std::mt19937_64 __e(__gen());

	MR_NODISCARD inline ll
	randi(ll l, ll r) mr_noexcept {
		uniform_int_distribution<ll> u(l, r);
		return u(__e);
	}

	MR_NODISCARD inline double
	randf(double l, double r) mr_noexcept {
		uniform_real_distribution<double> u(l, r);
		return u(__e);
	}

	template <typename T>
	struct uniform_auto_distribution_helper;

	template <typename T>
	using uniform_auto_distribution = typename uniform_auto_distribution_helper<T>::type;

	template <integral T>
	struct uniform_auto_distribution_helper<T> {
		using type = std::uniform_int_distribution<T>;
	};

	template <floating_point T>
	struct uniform_auto_distribution_helper<T> {
		using type = std::uniform_real_distribution<T>;
	};

	template <typename T>
	struct rand_generator {
		T l, r;
		uniform_auto_distribution<T> u;
		rand_generator(T l, T r) mr_noexcept : l(l), r(r), u{ l, r } {}
		T operator()()mr_noexcept{
			return u(__gen);
		}
	};

	inline void
	shuffle(auto begin, auto end) mr_noexcept {
		std::shuffle(begin, end, __gen);
	}

	inline void
	shuffle(auto&& r) mr_noexcept {
		ranges::shuffle(r, __gen);
	}
} // namespace mirai