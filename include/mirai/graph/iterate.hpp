#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/pipe.hpp>
#include <mirai/graph/graph.hpp>
#include <mirai/math/number.hpp>
#include <mirai/util/coro.hpp>
MR_NAMESPACE_BEGIN
template <auto& G, auto& config>
void calc_graph_degree() {
	for (ll i = G.node_count() - 1; i >= 0; i--) {
		if constexpr (requires { config.out_degree; }) config.out_degree[i] = G[i].size();
		if constexpr (requires { config.all_degree; }) config.all_degree[i] = G[i].size();
		if constexpr (
			requires { config.in_degree; } || requires { config.all_degree; })
			for (auto&& v : G[i] | transform(__edge_get_v)) {
				if constexpr (requires { config.in_degree; }) config.in_degree[v]++;
				if constexpr (requires { config.all_degree; }) config.all_degree[v]++;
			}
	}
}

template <typename T>
auto iterate_all_edges(const graph<T>& G)
	-> generator<std::conditional_t<std::is_same_v<T, void>, pair<ll, ll>, tuple<ll, ll, const T&>>> {
	for (auto i : views::iota(0ll, G.node_count())) {
		for (auto&& e : G[i]) {
			if constexpr (std::is_same_v<T, void>)
				co_yield { i, e };
			else
				co_yield { i, e.first, e.second };
		}
	}
}

generator<ll> topo_sort(auto&& G, auto&& in_degree, const auto&& get_v = __edge_get_v) {
	vector<ll> used_degree(G.node_count());
	queue<ll> q;
	for (auto i : views::iota(0ll, G.node_count()))
		if (in_degree[i] == 0) q.push(i);
	while (!q.empty()) {
		auto u = q.front();
		q.pop();
		// cout << "topo at:" << u << endl;
		co_yield u;
		for (auto&& v : G[u] | transform(get_v)) {
			if (++used_degree[v] == in_degree[v]) q.push(v);
		}
	}
}

auto dfs_in_tree(auto&& G, ll root, const auto&& get_v = __edge_get_v) {
	auto dfs = [&](auto&& self, auto&& G, ll u, ll fa) -> generator<tuple<ll, ll, ll>> {
		co_yield { u, fa, 0 };
		for (auto&& v : G[u] | transform(get_v) | filter(not_equal_to(fa)))
			for (auto&& [v2, f, status] : self(self, std::forward<decltype(G)>(G), v, u))
				co_yield { v2, f, status };
		co_yield { u, fa, 1 };
	};
	return dfs(std::forward<decltype(G)>(G), root, -1);
}
MR_NAMESPACE_END