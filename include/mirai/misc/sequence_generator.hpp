#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
template <typename Iterator>
void generate_recurrence_sequence(Iterator begin, Iterator end, auto&& v0, auto&& nxt) {
	*begin = v0;
	for (auto it = std::next(begin), lst = begin; it != end; ++it, ++lst) *it = nxt(*lst);
}

inline void generate_inv_sequence(std::random_access_iterator auto it, size_t n, ull P) mr_noexcept {
	it[1] = 1;
	auto p = it + 2;
	for (size_t i = 2; i <= n; ++i, ++p) {
		*p = (P - P / i) * it[P % i] % P;
	}
}
MR_NAMESPACE_END