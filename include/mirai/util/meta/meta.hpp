#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN

template <bool B, typename T, typename Y>
MR_NODISCARD constexpr decltype(auto) conditional_v(T&& t, Y&& y) mr_noexcept {
	if constexpr (B)
		return std::forward<T>(t);
	else
		return std::forward<Y>(y);
}
MR_NAMESPACE_END