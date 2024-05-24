#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
template <typename... Args>
MR_API void print_dbg(Args... args) {
	if constexpr (sizeof...(Args) > 0)
		((cout << args << " "), ...) << endl;
	else
		cout << endl;
}

#ifdef MR_DEBUG
#define debug(...) mirai::print_dbg("[", __func__, ":", __LINE__, "]", ##__VA_ARGS__) // NOLINT(bugprone-lambda-function-name)
#else
#define debug(...)
#endif

template <typename T, typename... Args>
MR_API std::ostream& operator<<(std::ostream& out, const tuple<T, Args...>& t) {
	[&]<size_t... I>(std::index_sequence<I...>) {
		out << "(" << std::get<0>(t), ((out << ", " << std::get<I + 1>(t)), ...), out << ")";
	}(std::make_index_sequence<sizeof...(Args)>());
	return out;
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
MR_NAMESPACE_END