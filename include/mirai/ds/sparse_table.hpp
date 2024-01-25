#pragma once
#include <mirai/pch.hpp>
#include <mirai/math/number.hpp>
#include <mirai/util/binary.hpp>
#include <mirai/util/range.hpp>
namespace mirai {
	template <typename T, class combine_proxy>
	class sparse_table {
	private:
		vector<vector<T>> val;

	public:
		inline void load(auto begin, auto end) mr_noexcept {
			ull n = std::distance(begin, end);
			ull layer = lg(n) + 1;
			val.resize(layer, vector<T>(n));
			copy(begin, end, val[0].begin());
			for (ull i = 1, k = 1; i < layer; i++, k <<= 1) {
				for (ull l = 0, r = k; r < n; l++, r++) {
					val[i][l] = combine_proxy::work(val[i - 1][l], val[i - 1][r]);
				}
			}
		}
		inline void load(range auto&& r) mr_noexcept { load(mr_begin(r), mr_end(r)); }
		sparse_table(auto begin, auto end) mr_noexcept { load(begin, end); }
		sparse_table(auto&& r) mr_noexcept : sparse_table(std::begin(r), std::end(r)) {}

		MR_NODISCARD inline T query(ull l, ull r) mr_noexcept {
			mr_assert(l <= r);
			ull layer = lg(r - l + 1);
			return combine_proxy::work(val[layer][l], val[layer][r - (1ull << layer) + 1]);
		}
	};

	namespace alias {
		template <typename T, class combine_proxy>
		using st = mirai::sparse_table<T, combine_proxy>;
	}
} // namespace mirai