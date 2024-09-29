#pragma once
#include <concepts>
#include <mirai/pch.hpp>
#include <mirai/util/range.hpp>
MR_NAMESPACE_BEGIN
template <typename T, auto&& pow_base, uint64_t mod>
	requires requires {{pow_base[1]}->std::convertible_to<T>; }
struct range_hash {
	static_assert(mod > 0);
	vector<uint64_t> data;
	template <typename src_iter>
	void generate(src_iter s_begin, src_iter s_end) mr_noexcept {
		data.resize(std::distance(s_begin, s_end));
		data[0] = *s_begin++;
		for (size_t i = 1; s_begin != s_end; ++i, ++s_begin) {
			data[i] = (data[i - 1] * pow_base[1] + *s_begin) % mod;
		}
	}

	void generate(range auto&& src) mr_noexcept {
		generate(mr_begin(src), mr_end(src));
	}

	range_hash() = default;

	template <typename src_iter>
	range_hash(src_iter s_begin, src_iter s_end) mr_noexcept {
		generate(s_begin, s_end);
	}

	explicit range_hash(range auto&& src) mr_noexcept {
		generate(std::forward<decltype(src)>(src));
	}

	MR_NODISCARD uint64_t get(size_t l, size_t r) const mr_noexcept {
		if (l == 0) return data[r];
		return (data[r] - data[l - 1] * pow_base[r - l + 1] % mod + mod) % mod;
	}
};
MR_NAMESPACE_END