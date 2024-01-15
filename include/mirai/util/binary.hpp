#pragma once
#include <mirai/pch.hpp>
namespace mirai {
	inline ll clzll(ull x) mr_noexcept {
		mr_assert(x);
		return __builtin_clzll(x);
	}

	inline ll popcntll(ull x) mr_noexcept {
		return __builtin_popcountll(x);
	}
} // namespace mirai