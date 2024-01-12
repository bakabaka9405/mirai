#pragma once
#include "../pch.hpp"
namespace mirai{
	[[nodiscard]] inline 
	ll time_now()mr_noexcept{
		return chrono::system_clock::now().time_since_epoch().count();
	}

	inline
	random_device __gen;

	inline 
	std::mt19937_64 __e(__gen());


	[[nodiscard]] inline
	ll randi(ll l,ll r)mr_noexcept{
		uniform_int_distribution<ll> u(l,r);
		return u(__e);
	}

	[[nodiscard]] inline
	double randf(double l,double r)mr_noexcept{
		uniform_real_distribution<double> u(l,r);
		return u(__e);
	}

	inline
	void shuffle(auto begin,auto end)mr_noexcept{
		std::shuffle(begin,end,__gen);
	}

	inline
	void shuffle(auto&& r)mr_noexcept{
		ranges::shuffle(r,__gen);
		//shuffle(std::begin(r),std::end(r));
	}
}