#pragma once
#include <mirai/pch.hpp>
namespace mirai {
	enum graph_model_type {
		vector_model,
		link_model
	};

	template <typename T, graph_model_type _type = vector_model>
	class graph;

	template <typename T>
	class graph<T, vector_model> {
	public:
		struct edge {
			ll v;
			T data;
		};
		graph() = delete;
		inline graph(ll n) mr_noexcept : _e(n) {}
		void insert(ll u, ll v, const T& data) mr_noexcept {
			_e[u].emplace_back(v, data);
		}
		inline decltype(auto) operator[](size_t x) mr_noexcept { return _e[x]; }
		inline size_t node_count() const { return _e.size(); }

	private:
		vector<vector<edge>> _e;
	};

	template <>
	class graph<void, vector_model> {
	public:
		using edge = ll;
		graph() = delete;
		inline graph(ll n) mr_noexcept : _e(n) {}
		void insert(ll u, ll v) mr_noexcept {
			_e[u].emplace_back(v);
		}
		inline decltype(auto) operator[](size_t x) mr_noexcept { return _e[x]; }

		inline size_t node_count() const { return _e.size(); }

	private:
		vector<vector<edge>> _e;
	};

	template <typename T>
	class graph<T, link_model> {
	public:
		struct edge {
			ll v;
			T data;
		};
		graph() = delete;
		inline graph(ll n) mr_noexcept : _head(n, -1), _nxt{}, _e{} {}
		void insert(ll u, ll v, const T& data) {
			_nxt.push_back(_head[u]);
			_head[u] = _e.size();
			_e.emplace_back(v, data);
		}
		inline auto operator[](size_t x) mr_noexcept {
			struct range_wrapper {
				ll _start;
				vector<ll>& _nxt;
				vector<edge>& _e;
				struct iterator {
					ll _index;
					vector<ll>& _nxt;
					vector<edge>& _e;
					inline bool operator!=(const iterator& rt) const mr_noexcept { return _index != rt._index; }
					inline void operator++() mr_noexcept { _index = _nxt[_index]; }
					inline auto operator*() const mr_noexcept {
						return _e[_index];
					}
				};
				inline auto begin() const mr_noexcept {
					return iterator{ _start, _nxt, _e };
				}
				inline auto end() const mr_noexcept {
					return iterator{ -1, _nxt, _e };
				}
			};
			return range_wrapper{ _head[x], _nxt, _e };
		}
		inline size_t node_count() const { return _head.size(); }

	private:
		vector<ll> _head;
		vector<ll> _nxt;
		vector<edge> _e;
	};
	template <>
	class graph<void, link_model> {
	public:
		using edge = ll;
		graph() = delete;
		inline graph(ll n) mr_noexcept : _head(n, -1), _nxt{}, _e{} {}
		void insert(ll u, ll v) {
			_nxt.push_back(_head[u]);
			_head[u] = _e.size();
			_e.emplace_back(v);
		}
		inline auto operator[](size_t x) mr_noexcept {
			struct range_wrapper {
				ll _start;
				vector<ll>& _nxt;
				vector<edge>& _e;
				struct iterator {
					ll _index;
					vector<ll>& _nxt;
					vector<edge>& _e;
					inline bool operator!=(const iterator& rt) const mr_noexcept { return _index != rt._index; }
					inline void operator++() mr_noexcept { _index = _nxt[_index]; }
					inline auto operator*() const mr_noexcept {
						return _e[_index];
					}
				};
				inline auto begin() const mr_noexcept {
					return iterator{ _start, _nxt, _e };
				}
				inline auto end() const mr_noexcept {
					return iterator{ -1, _nxt, _e };
				}
			};
			return range_wrapper{ _head[x], _nxt, _e };
		}
		inline size_t node_count() const { return _head.size(); }

	private:
		vector<ll> _head;
		vector<ll> _nxt;
		vector<edge> _e;
	};
} // namespace mirai

namespace std {

} // namespace std