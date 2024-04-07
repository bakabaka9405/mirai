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

// clang-format off
	template <auto& G, auto& config,typename Queue=queue<ll>>
		requires requires { config.in_degree; } && 
				(requires { config.topo_n; } || requires { config.topo_order; })
// clang-format on
void topo_sort() {
	const ll node_count = G.node_count();
	vector<ll> used_deg(node_count);
	Queue q;
	for (ll i : views::iota(0, node_count) | views::filter([](int x) { return config.in_degree[x] == 0; })) q.push(i);
	ll cnt = 0;
	while (!q.empty()) {
		ll u = [&]() {if constexpr(requires{q.front();})return q.front();else return q.top(); }();
		q.pop();
		++cnt;
		if constexpr (requires { config.topo_n; }) config.topo_n[u] = cnt;
		if constexpr (requires { config.topo_order; }) {
			if constexpr (requires { config.topo_order_pt; })
				config.topo_order[config.topo_order_pt++] = u;
			else
				config.topo_order.push_back(u);
		}
		for (auto&& v : G[u] | transform(__edge_get_v)) {
			if (++used_deg[v] == config.in_degree[v]) q.push(v);
		}
	}
}

generator<pair<ll, ll>> __dfs_in_tree_impl(auto&& G, ll u, ll fa) {
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