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
		ull size = 0, layer = 0;

	public:
		inline void resize(size_t n) mr_noexcept {
			size = n;
			layer = lg(size) + 1;
			val.resize(layer);
		}
		inline void load(auto begin, auto end) mr_noexcept {
			resize(std::distance(begin, end));
			copy(begin, end, val[0].begin());
		}
		inline void calc() mr_noexcept {
			for (ull i = 1, k = 1; i < layer; i++, k <<= 1u) {
				val[i].resize(size - k + 1);
				for (ull l = 0, r = k; r < size; l++, r++) {
					val[i][l] = combine_proxy::work(val[i - 1][l], val[i - 1][r]);
				}
			}
		}
		inline void load(range auto&& r) mr_noexcept { load(mr_begin(r), mr_end(r)); }
		sparse_table() mr_noexcept = default;
		sparse_table(auto begin, auto end) mr_noexcept { load(begin, end); }
		inline T& operator[](ull pos) mr_noexcept { return val[0][pos]; }
		MR_NODISCARD inline T query(ull l, ull r) mr_noexcept {
			MR_ASSUME(l <= r);
			ull layer = lg(r - l + 1);
			return combine_proxy::work(val[layer][l], val[layer][r - (1ull << layer) + 1]);
		}
	};

	namespace alias {
		template <typename T, class combine_proxy>
		using st = mirai::sparse_table<T, combine_proxy>;
	}
} // namespace mirai