#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/constexpr_str.hpp>
namespace mirai {
	template <typename T, typename... Args>
	struct type_name_helper;

	template <typename T, typename... Args>
	constexpr constexpr_str type_name = string_concat(type_name<T>, string_literal(", "), type_name<Args...>);

	template <typename T>
	constexpr constexpr_str type_name<T> = type_name_helper<T>::name;

#define ADD_TYPE_NAME(x, y, z)                          \
	template <x>                                        \
	struct type_name_helper<y> {                        \
		constexpr static auto name = string_literal(z); \
	}

#define COMMA ,
#define DOTS ...

	ADD_TYPE_NAME(typename T, T&, string_concat(type_name<T>, string_literal("&")));
	ADD_TYPE_NAME(typename T, T*, string_concat(type_name<T>, string_literal("*")));
	ADD_TYPE_NAME(typename T, T&&, string_concat(type_name<T>, string_literal("&&")));
	ADD_TYPE_NAME(, int, string_literal("int"));
	ADD_TYPE_NAME(, long long, string_literal("long long"));
	ADD_TYPE_NAME(, double, string_literal("double"));
	ADD_TYPE_NAME(typename T COMMA typename Y, pair<T COMMA Y>,
				  string_concat(string_literal("pair<"), type_name<T>, string_literal(", "), type_name<Y>, string_literal(">")));
	ADD_TYPE_NAME(typename DOTS Args, tuple<Args DOTS>,
				  string_concat(string_literal("tuple<"),type_name<Args DOTS>, string_literal(">")));

	constexpr string get_type_name(auto&& x) {
		return type_name<decltype(x)>.data;
	}

#undef ADD_TYPE_NAME
#undef COMMA
#undef DOTS
} // namespace mirai