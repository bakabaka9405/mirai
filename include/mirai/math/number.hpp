#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/binary.hpp>
namespace mirai {
	[[maybe_unused]] inline constexpr ll
		pow10[] = { 1ll,
					10ll,
					100ll,
					1000ll,
					10000ll,
					100000ll,
					1000000ll,
					10000000ll,
					100000000ll,
					1000000000ll,
					10000000000ll,
					100000000000ll,
					1000000000000ll,
					10000000000000ll,
					100000000000000ll,
					1000000000000000ll,
					10000000000000000ll,
					100000000000000000ll,
					1000000000000000000ll };

	inline consteval ll pow2(ll x) {
		return 1ll << x;
	}

	inline ull lg(ull n) mr_noexcept {
		return 63 - clzll(n);
	}
} // namespace mirai