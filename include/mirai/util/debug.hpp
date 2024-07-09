#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
template <typename T, typename... Args>
MR_API std::ostream& operator<<(std::ostream& out, const tuple<T, Args...>& t) {
	[&]<size_t... I>(std::index_sequence<I...>) {
		out << "(" << std::get<0>(t), ((out << ", " << std::get<I + 1>(t)), ...), out << ")";
	}(std::make_index_sequence<sizeof...(Args)>());
	return out;
}

template <typename... Args>
MR_API std::istream& operator>>(std::istream& in, tuple<Args...>& t) {
	[&]<size_t... I>(std::index_sequence<I...>) {
		((in >> std::get<I>(t)), ...);
	}(std::make_index_sequence<sizeof...(Args)>());
	return in;
}

template <typename T>
MR_API std::ostream& operator<<(std::ostream& out, const vector<T>& vec) {
	out << '[';
	if (!vec.empty()) {
		out << vec.front();
		for (size_t i = 1; i < vec.size(); i++) {
			out << ", " << vec[i];
		}
	}
	out << ']';
	return out;
}

template <typename T>
MR_API std::ostream& operator<<(std::ostream& out, const std::set<T>& s) {
	out << '{';
	if (!s.empty()) {
		auto it = s.begin();
		out << *it;
		for (++it; it != s.end(); ++it) {
			out << ", " << *it;
		}
	}
	out << '}';
	return out;
}

template <typename T, typename Y>
MR_API std::ostream& operator<<(std::ostream& out, const std::map<T, Y>& m) {
	out << '{';
	if (!m.empty()) {
		auto it = m.begin();
		out << it->first << ": " << it->second;
		for (++it; it != m.end(); ++it) {
			out << ", " << it->first << ": " << it->second;
		}
	}
	out << '}';
	return out;
}

template <typename... Args>
MR_API void __print_dbg_impl(const std::source_location& location, Args... args) {
	if constexpr (sizeof...(Args) > 0) {
		cout << location.file_name() << ":" << location.line() << ":" << location.column() << ": `" << location.function_name() << "`: ";
		cout<<"\033[36m";
		((cout << args << " "), ...) << endl;
		cout<<"\033[0m";
	}
	else
		cout << endl;
}

#ifdef MR_DEBUG
#define debug(...) mirai::__print_dbg_impl(std::source_location::current(), __VA_ARGS__)
#else
#define debug(...)
#endif
MR_NAMESPACE_END