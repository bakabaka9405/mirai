#pragma once
#include <mirai/pch.hpp>
namespace mirai {
	template <typename T>
	concept vector_like = requires(T t1, T t2) {
		{ t1 + t2 } -> std::same_as<T>;
		{ -t1 } -> std::same_as<T>;
		t1.norm();
	};

	template <typename T>
	concept vector_like_or_void = vector_like<T> || std::same_as<T, void>;

	template <vector_like_or_void vec_t>
	class dsu;

	template <vector_like vec_t>
	class dsu<vec_t> {
	private:
		vector<ll> _fa;
		vector<vec_t> _diff;

	public:
		inline dsu(ll n) mr_noexcept : _fa(n), _diff(n) {
			ranges::iota(_fa, 0);
		}
		inline ll find(ll x) mr_noexcept {
			if (x == _fa[x]) return x;
			find(_fa[x]);
			_diff[x] = _diff[x] + _diff[_fa[x]];
			return _fa[x] = _fa[_fa[x]];
		}
		inline ll merge(ll x, ll y, vec_t diff) mr_noexcept {
			ll x_fa = find(x), y_fa = find(y);
			_fa[x_fa] = y_fa;
			_diff[x_fa] = -_diff[x] + diff + _diff[y];
		}
		inline auto query(ll x, ll y) mr_noexcept {
			ll x_fa = find(x), y_fa = find(y);
			mr_assert(x_fa == y_fa);
			return (_diff[x] - _diff[y]).norm();
		}
	};

	template <>
	class dsu<void> {
	private:
		vector<ll> _fa;

	public:
		inline dsu(ll n) mr_noexcept : _fa(n) { ranges::iota(_fa, 0); }
		inline ll find(ll x) mr_noexcept {
			while (x != _fa[x]) x = _fa[x] = _fa[_fa[x]];
			return x;
		}
		inline void merge(ll x, ll y) mr_noexcept { _fa[find(x)] = find(y); }
		inline bool query(ll x, ll y) mr_noexcept { return find(x) == find(y); }
	};

	using simple_dsu = dsu<void>;
} // namespace mirai::beta