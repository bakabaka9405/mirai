#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/range.hpp>
namespace mirai {
	enum graph_model_type {
		vector_model,
		link_model
	};

	template <typename T, graph_model_type _type = vector_model>
	class graph {
	private:
		constexpr static bool _is_vector_model = _type == vector_model;
		constexpr static bool _is_link_model = _type == link_model;
		constexpr static bool _have_weight = !std::is_same_v<T, void>;
		using edge = std::conditional_t<_have_weight, pair<ll, T>, ll>;
		std::conditional_t<_is_vector_model, vector<vector<edge>>, vector<pair<edge, ll>>> _e;
		struct __empty_item {
			__empty_item(ll, ll) {}
		};
		[[no_unique_address]] std::conditional_t<_is_link_model, vector<ll>, __empty_item> _head; // NOLINT
	public:
		inline graph() mr_noexcept : _e(), _head(0, -1) {}
		inline graph(size_t n) mr_noexcept : _e(_is_vector_model ? n : 0), _head(n, -1) {}
		inline void resize(size_t n) mr_noexcept {
			if constexpr (_is_vector_model)
				_e.resize(n);
			else
				_head.resize(n, -1);
		}
		template <typename U = T, typename std::enable_if_t<!std::is_same_v<U, void>, int> = 0>
		void insert(ll u, ll v, const std::conditional_t<_have_weight, ll, U>& data) mr_noexcept {
			if (node_count() <= max(u, v)) resize(max(u, v) + 1);
			if constexpr (_is_vector_model)
				_e[u].emplace_back(v, data);
			else
				_e.emplace_back(make_pair(v, data), std::exchange(_head[u], _e.size()));
		}
		template <typename U = T, typename std::enable_if_t<std::is_same_v<U, void>, int> = 0>
		void insert(ll u, ll v) mr_noexcept {
			if (node_count() <= max(u, v)) resize(max(u, v) + 1);
			if constexpr (_is_vector_model)
				_e[u].push_back(v);
			else
				_e.emplace_back(v, std::exchange(_head[u], _e.size()));
		}
		template <typename U = T, typename std::enable_if_t<!std::is_same_v<U, void>, int> = 0>
		void insert_bothway(ll u, ll v, const std::conditional_t<_have_weight, ll, U>& data) mr_noexcept {
			insert(u, v, data);
			insert(v, u, data);
		}
		template <typename U = T, typename std::enable_if_t<std::is_same_v<U, void>, int> = 0>
		void insert_bothway(ll u, ll v) mr_noexcept {
			insert(u, v);
			insert(v, u);
		}
		decltype(auto) operator[](size_t u) const mr_noexcept {
			if constexpr (_is_vector_model)
				return _e[u];
			else {
				struct range_wrapper {
					const ll _start;
					const vector<pair<edge, ll>>& _e;
					struct iterator {
						ll _index;
						const vector<pair<edge, ll>>& _e;
						inline bool operator!=(const std::default_sentinel_t& rt) const mr_noexcept { return _index != -1; }
						inline iterator& operator++() mr_noexcept {
							_index = _e[_index].second;
							return *this;
						}
						inline iterator operator++(int) mr_noexcept { return iterator{ std::exchange(_index, _e[_index].second), _e }; }
						inline auto operator*() const mr_noexcept { return _e[_index].first; }
					};
					inline auto begin() const mr_noexcept { return iterator{ _start, _e }; }
					inline auto end() const mr_noexcept { return std::default_sentinel_t{}; }
					inline size_t size() const mr_noexcept {
						size_t res = 0;
						ll x = _start;
						while (~x) res++, x = _e[x].second;
						return res;
					}
					inline bool empty() const mr_noexcept { return _start != -1; }
				};
				return range_wrapper{ _head[u], _e };
			}
		}
		inline ll node_count() const mr_noexcept {
			if constexpr (_is_vector_model)
				return _e.size();
			else
				return _head.size();
		}
	};

	template <typename T>
	concept weighted_graph = !std::same_as<T, void>;

} // namespace mirai
