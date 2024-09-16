#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
template <typename Iterator>
void generate_recurrence_sequence(Iterator begin, Iterator end, auto&& v0, auto&& nxt) {
	*begin = v0;
	for (auto it = std::next(begin), lst = begin; it != end; ++it, ++lst) *it = nxt(*lst);
}
MR_NAMESPACE_END