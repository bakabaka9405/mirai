#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
inline void to_max(auto& a, auto&& b) mr_noexcept {
	a = std::max(a, b);
}
inline void to_min(auto& a, auto&& b) mr_noexcept {
	a = std::min(a, b);
}
MR_NAMESPACE_END