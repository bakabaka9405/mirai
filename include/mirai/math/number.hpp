#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/binary.hpp>
MR_NAMESPACE_BEGIN
[[maybe_unused]] inline constexpr std::array<ll, 19>
	pow10{ 1ll,
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

inline consteval ull pow2(ull x) {
	return 1ull << x;
}

inline ull lg(ull n) mr_noexcept {
	return std::bit_width(n);
}
MR_NAMESPACE_END