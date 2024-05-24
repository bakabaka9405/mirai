#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
inline void to_max(auto& a, auto&& b) mr_noexcept {
	a = std::max(a, b);
}
inline void to_min(auto& a, auto&& b) mr_noexcept {
	a = std::min(a, b);
}

template <typename T = ll>
constexpr auto make_mdvector(auto&& n, auto&&... m) {
	if constexpr (sizeof...(m) == 0) {
		return vector<T>(n);
	}
	else {
		using Y = decltype(make_mdvector<T>(m...));
		return vector<Y>(n, make_mdvector<T>(m...));
	}
}

MR_NAMESPACE_END