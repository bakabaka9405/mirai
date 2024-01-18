#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/constexpr_str.hpp>
namespace mirai {
	template <typename T, typename... Args>
	struct type_name_helper{
		constexpr static auto name=string_literal("<unknown-type>");
	};

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

	ADD_TYPE_NAME(typename T, T&, string_concat(type_name<T>, "&"));
	ADD_TYPE_NAME(typename T, T*, string_concat(type_name<T>, "*"));
	ADD_TYPE_NAME(typename T, T&&, string_concat(type_name<T>, "&&"));
	ADD_TYPE_NAME(typename T, const T, string_concat("const ", type_name<T>));
	ADD_TYPE_NAME(, char, "char");
	ADD_TYPE_NAME(, int, "int");
	ADD_TYPE_NAME(, long long, "long long");
	ADD_TYPE_NAME(, double, "double");
	ADD_TYPE_NAME(, string, "string");
	ADD_TYPE_NAME(, string_view, "string_view");
	ADD_TYPE_NAME(, std::istream, "istream");
	ADD_TYPE_NAME(, std::ostream, "ostream");
	ADD_TYPE_NAME(typename T, vector<T>, string_concat("vector<", type_name<T>, ">"));
	ADD_TYPE_NAME(typename T, queue<T>, string_concat("queue<", type_name<T>, ">"));
	ADD_TYPE_NAME(typename T, set<T>, string_concat("set<", type_name<T>, ">"));
	ADD_TYPE_NAME(typename T COMMA typename Y, map<T COMMA Y>, string_concat("map<", type_name<T>, ", ", type_name<Y>, ">"));
	ADD_TYPE_NAME(typename T COMMA typename Y, pair<T COMMA Y>,
				  string_concat(string_literal("pair<"), type_name<T>, string_literal(", "), type_name<Y>, string_literal(">")));
	ADD_TYPE_NAME(typename DOTS Args, tuple<Args DOTS>,
				  string_concat(string_literal("tuple<"), type_name<Args DOTS>, string_literal(">")));

	template <typename T>
	constexpr string get_type_name(T&& x) {
		return type_name<decltype(x)>.data;
	}

	template <typename T, typename... Args>
	constexpr string get_type_name(T&& x, Args&&... args) {
		return get_type_name(std::forward<T>(x)) + ", " + get_type_name(std::forward<Args...>(args...));
	}
} // namespace mirai