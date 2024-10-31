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

public:
	inline void resize(size_t n) mr_noexcept {
		size = n;
		layer = std::bit_width(n) + 1;
		val.resize(layer);
	}
	inline void load(auto&& begin, auto&& end) mr_noexcept {
		if constexpr (requires { std::distance(begin, end); }) {
			resize(std::distance(begin, end));
			val[0].resize(std::distance(begin, end));
			copy(begin, end, val[0].begin());
		}
		else {
			if (val.empty()) val.push_back({});
			val[0].clear();
			for (auto i = begin; i != end; ++i) val[0].push_back(*i);
			resize(val[0].size());
		}
	}
	inline void calc() mr_noexcept {
		for (ull i = 1, k = 1; i < layer; i++, k <<= 1u) {
			val[i].resize(size - k + 1);
			for (ull l = 0, r = k; r < val[i - 1].size(); l++, r++) {
				val[i][l] = merge(val[i - 1][l], val[i - 1][r]);
			}
		}
	}
	inline void load(range auto&& r) mr_noexcept { load(mr_begin(r), mr_end(r)); }
	inline void load_and_calc(range auto&& r) mr_noexcept {
		load(r);
		calc();
	}
	inline void load_and_calc(auto&& begin, auto&& end) mr_noexcept {
		load(begin, end);
		calc();
	}
	sparse_table() mr_noexcept = default;
	sparse_table(auto begin, auto end) mr_noexcept { load(begin, end); }
	inline T& operator[](ull pos) mr_noexcept { return val[0][pos]; }
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