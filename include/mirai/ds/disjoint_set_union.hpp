#pragma once
#include "mirai/define.hpp"
#include <mirai/pch.hpp>
namespace mirai {
	template <typename T>
	concept vector_like = requires(T t1, T t2) {
		T{};
		{ t1 + t2 } -> std::same_as<T>;
		{ -t1 } -> std::same_as<T>;
		{ t1 == t2 } -> std::same_as<bool>;
	};

	template <vector_like diff_t = ll, bool Compression = true, bool MergeByRank = true>
	class disjoint_set_union {
	private:
		vector<ll> _fa;
		vector<diff_t> _diff;
		vector<ll> _rank;
		inline void trace_to_top(ll& x, diff_t& diff) {
			if constexpr (Compression) {
				diff = _diff[std::exchange(x, find(x))];
			}
			else {
				diff = _diff[x];
				while (x != _fa[x]) diff = diff + _diff[x = _fa[x]];
			}
		}

	public:
		inline disjoint_set_union(ll n) mr_noexcept : _fa(n), _diff(n), _rank(n, 1) {
			std::iota(_fa.begin(), _fa.end(), 0);
		}
		inline ll find(ll x) mr_noexcept {
			ll fa = _fa[x];
			if (x == fa) return x;
			ll top = find(_fa[x]);
			if constexpr (Compression) {
				_diff[x] = _diff[x] + _diff[fa];
				_fa[x] = top;
			}
			return top;
		}
		inline void merge(ll x, ll y, diff_t diff = diff_t{}) mr_noexcept {
			diff_t diff_x, diff_y;
			trace_to_top(x, diff_x);
			trace_to_top(y, diff_y);
			diff = (-diff_x) + diff + diff_y;
			if constexpr (MergeByRank) {
				if (_rank[x] > _rank[y]) swap(x, y), diff = -diff;
			}
			_rank[y] += _rank[x];
			_fa[x] = y;
		}
		inline auto query(ll x, ll y) mr_noexcept {
			diff_t diff_x, diff_y;
			trace_to_top(x, diff_x);
			trace_to_top(y, diff_y);
			mr_assert(x == y);
			return diff_x + (-diff_y);
		}
		inline bool try_merge(ll x, ll y, diff_t diff = diff_t{}) mr_noexcept {
			if (find(x) == find(y)) {
				if (query(x, y) == diff)
					return true;
				else
					return false;
			}
			else {
				merge(x, y, diff);
				return true;
			}
		}
		inline ll get_rank(ll x) mr_noexcept {
			return _rank[find(x)];
		}
	};

	namespace alias {
		template <vector_like diff_t = ll, bool Compression = true, bool MergeByRank = true>
		using dsu = mirai::disjoint_set_union<diff_t, Compression, MergeByRank>;
	}
} // namespace mirai