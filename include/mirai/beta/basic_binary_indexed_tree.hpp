#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/binary.hpp>
namespace mirai {
	template <typename T = ll>
	class basic_binary_indexed_tree {
	private:
		std::vector<T> tr;

	public:
		basic_binary_indexed_tree() {}
		basic_binary_indexed_tree(std::vector<T> a) {
			tr.resize(a.size() + 1);
			for (T i = 1; i <= a.size(); i++) {
				tr[i] += a[i];
				if (T j = i + lowbit(i); j <= a.size()) tr[j] += tr[i];
			}
		}
		template <size_t N>
		basic_binary_indexed_tree(std::array<T, N + 1> a) {
			tr.resize(N + 1);
			for (T i = 1; i <= N; i++) {
				tr[i] += a[i];
				if (T j = i + lowbit(i); j <= N) tr[j] += tr[i];
			}
		}
		template <size_t N>
		basic_binary_indexed_tree(T (&a)[N+1], ll n = N) {
			tr.resize(n + 1);
			for (T i = 1; i <= n; i++) {
				tr[i] += a[i];
				if (T j = i + lowbit(i); j <= n) tr[j] += tr[i];
			}
		}
		void add(T x, T v) {
			while (x <= tr.size() - 1) tr[x] += v, x += lowbit(x);
		}
		T query(T x) {
			T res;
			while (x) res += tr[x], x -= lowbit(x);
			return res;
		}
		T query(T l, T r) {
			return query(r) - query(l - 1);
		}
		void resize(size_t size) {
			tr.resize(size + 1);
		}
	};
	namespace alias{
		template <typename T = ll>
		using BIT = basic_binary_indexed_tree<T>;
	}
} // namespace mirai