#pragma once
#include <mirai/pch.hpp>
namespace mirai {
	template <class T, class Tuple, size_t... I>
	T _tuple_to_struct_impl(Tuple&& t, std::index_sequence<I...>) {
		return { std::get<I...>(t) };
	}

	template <typename T, typename Tuple>
	T tuple_to_struct(Tuple&& t) {
		return _tuple_to_struct_impl(std::forward<Tuple>(t), std::make_index_sequence<std::tuple_size_v<Tuple>>());
	}
} // namespace mirai