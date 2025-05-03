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

template <size_t I, typename... T>
struct nth_type {
	using type = std::tuple_element_t<I, std::tuple<T...>>;
};

template <size_t I, typename... T>
using nth_type_t = typename nth_type<I, T...>::type;
MR_NAMESPACE_END