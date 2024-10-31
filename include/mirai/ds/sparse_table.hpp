#pragma once
#include <mirai/pch.hpp>
#include <mirai/math/number.hpp>
#include <mirai/util/binary.hpp>
#include <mirai/util/range.hpp>
MR_NAMESPACE_BEGIN
template <typename T, class merger>
	requires requires(merger merge, T a, T b) { {merge(a,b)}->std::same_as<T>; }
class sparse_table {
private:
	constexpr static merger merge{};
	vector<vector<T>> val;
	ull size = 0, layer = 0;

	inline void calc() mr_noexcept {
		layer = std::bit_width(size);
		val.resize(layer);
		for (ull i = 1, k = 1; i < layer; i++, k <<= 1) {
			val[i].resize(size - k + 1);
			for (ull l = 0, r = k; r < size; l++, r++)
				val[i][l] = merge(val[i - 1][l], val[i - 1][r]);
		}
	}

public:
	constexpr sparse_table() = default;
	explicit constexpr sparse_table(const vector<T>& vec)
		: size(vec.size()), val(1, vec) {
		calc();
	}
	explicit constexpr sparse_table(vector<T>&& vec)
		: size(vec.size()), val(1, std::move(vec)) {
		calc();
	}
	MR_NODISCARD inline T query(ull l, ull r) mr_noexcept {
		MR_ASSUME(l <= r);
		ull layer = std::bit_width(r - l + 1) - 1;
		return merge(val[layer][l], val[layer][r - (1ull << layer) + 1]);
	}
};

namespace alias {
	template <typename T, class merger>
	using st = mirai::sparse_table<T, merger>;
}
MR_NAMESPACE_END