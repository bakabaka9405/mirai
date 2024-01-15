#pragma once
#include <mirai/pch.hpp>
#include <mirai/math/number.hpp>
#include <mirai/util/binary.hpp>
namespace mirai {
	template <typename T, class combine_proxy>
	class sparse_table {
	private:
		vector<vector<T>> val;

	public:
		inline void load(auto begin, auto end) mr_noexcept {
			ull n = std::distance(begin, end);
			ull layer = lg(n) + 1;
			val.resize(layer);
			for (auto& i : val) i.resize(n);
			copy(begin, end, val[0].begin());
			for (ull i = 1, k = 1; i < layer; i++, k <<= 1) {
				for (ull l = 0, r = k; r < n; l++, r++) {
					val[i][l] = combine_proxy::work(val[i - 1][l], val[i - 1][r]);
				}
			}
		}
		sparse_table(auto begin, auto end) mr_noexcept {
			load(begin, end);
		}

		MR_NODISCARD inline T query(ull l, ull r) mr_noexcept {
			ull layer = lg(r - l + 1);
			return combine_proxy::work(val[layer][l], val[layer][r - (1ull << layer) + 1]);
		}
	};
} // namespace mirai