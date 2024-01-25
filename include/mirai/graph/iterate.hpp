#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/pipeline.hpp>
#include <mirai/graph/graph.hpp>
#include <mirai/math/number.hpp>
namespace mirai {
	inline constexpr auto __edge_get_v = [](auto&& edge) {
		if constexpr (std::is_same_v<std::decay_t<decltype(edge)>, ll>)
			return edge;
		else
			return edge.first;
	};
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

	template <auto& G, auto& config>
	void __dfs_in_tree_impl(ll u, ll fa, ll t) {
		if constexpr (requires { config.fa; }) {
			if constexpr (requires { config.fa[u][0]; }) {
				config.fa[u][0] = fa;
				for (auto i : views::iota(1) | take(lg(G.node_count())))
					config.fa[u][i] = config.fa[config.fa[u][i - 1]][i - 1];
			}
			else
				config.fa[u] = fa;
		}
		if constexpr (requires { config.depth; }) config.depth[u] = config.depth[fa] + 1;
		if constexpr (requires { config.dfn; }) config.dfn[u] = t;
		if constexpr (requires { config.size; }) config.size[u] = 1;
		if constexpr (requires { config.dfs_order; }) {
			if constexpr (requires { config.dfs_order_pt; })
				config.dfs_order[config.dfs_order_pt++] = u;
			else
				config.dfs_order.push_back(u);
		}
		if constexpr (requires { config.euler_order; }) {
			if constexpr (requires { config.euler_order_pt; })
				config.euler_order[config.euler_order_pt++] = u;
			else
				config.euler_order.push_back(u);
		}
		for (auto&& v : G[u] | transform(__edge_get_v)) {
			[[unlikely]] if (v == fa)
				continue;
			__dfs_in_tree_impl<G, config>(v, u, t + 1);
			if constexpr (requires { config.size; }) config.size[u] += config.size[v];
		}
		if constexpr (requires { config.dfn;config.lev; }) config.lev[u] = t;
		if constexpr (requires { config.euler_order; }) {
			if constexpr (requires { config.euler_order_pt; })
				config.euler_order[config.euler_order_pt++] = u;
			else
				config.euler_order.push_back(u);
		}
	}

	template <auto& G, auto& config>
	void dfs_in_tree(ll start) {
		__dfs_in_tree_impl<G, config>(start, start, 0);
	}
} // namespace mirai