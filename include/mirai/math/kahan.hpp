#pragma once
#include <mirai/pch.hpp>
namespace mirai {
	class kahan {
	private:
		double sum = 0, c = 0;

	public:
		kahan() = default;
		kahan(double v)
			: sum{ v }, c{ 0 } {}
		inline kahan operator+(double v) const mr_noexcept {
			kahan res{ *this };
			return res += v;
		}
		inline kahan& operator+=(double v) mr_noexcept {
			double y = v - c;
			double t = sum + y;
			c = (t - sum) - y;
			sum = t;
			return *this;
		}
		inline double operator*() const mr_noexcept { return sum; }
	};
} // namespace mirai