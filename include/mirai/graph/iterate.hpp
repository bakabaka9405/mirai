#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/pipeline.hpp>
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

generator<ll> topo_sort(auto&& G, auto&& in_degree) {
	vector<ll> used_degree(G.node_count());
	queue<ll> q;
	for (auto i : views::iota(0ll, G.node_count()))
		if (in_degree[i] == 0) q.push(i);
	while (!q.empty()) {
		auto u = q.front();
		q.pop();
		//cout << "topo at:" << u << endl;
		co_yield u;
		for (auto&& v : G[u] | transform(__edge_get_v)) {
			if (++used_degree[v] == in_degree[v]) q.push(v);
		}
	}
}

generator<pair<ll, ll>> __dfs_in_tree_impl(auto&& G, ll u, ll fa) { // NOLINT(bugprone-reserved-identifier)
	co_yield { u, 0 };
	for (auto&& v : G[u] | transform(__edge_get_v) | filter(not_equal_to(fa)))
		for (auto&& [v2, status] : __dfs_in_tree_impl(std::forward<decltype(G)>(G), v, u))
			co_yield { v2, status };
	co_yield { u, 1 };
}

auto dfs_in_tree(auto&& G, ll root) {
	return __dfs_in_tree_impl(std::forward<decltype(G)>(G), root, -1);
}
MR_NAMESPACE_END