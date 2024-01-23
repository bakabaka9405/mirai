#pragma once
#include <mirai/pch.hpp>
#include <mirai/graph/graph_model.hpp>
namespace mirai {
	template <auto& G, auto& config>
	void calc_graph_degree() {
		for (ll i = G.node_count; i >= 0; i--) {
			if constexpr (requires { config.out_degree; }) config.out_degree[i] = G[i].size();
			if constexpr (requires { config.all_degree; }) config.all_degree[i] = G[i].size();
			// if constexpr(requires{config.in_degree;}||requires{config.all_degree;})
			// for(auto)
		}
	}

	template <auto& G, auto& config>
	void __dfs_in_tree_impl(ll u, ll fa) {
		if constexpr (requires { config.fa; }) config.fa[u] = fa;
		if constexpr (requires { config.dfn;config.timestamp; }) config.dfn[u] = ++config.timestamp;
		if constexpr (requires { config.size; }) config.size[u] = 1;
		for (auto&& v : G[u] | views::transform([]<typename T>(T&& edge) {
							if constexpr (std::is_same_v<std::decay_t<T>, ll>)
								return edge;
							else
								return edge.first;
						})) {
			[[unlikely]] if (v == fa)
				continue;
			__dfs_in_tree_impl<G, config>(v, u);
			if constexpr (requires { config.size; }) config.size[u] += config.size[v];
		}
		if constexpr (requires { config.dfn;config.lev;config.timestamp; }) config.lev[u] = config.timestamp;
	}

	template <auto& G, auto& config>
	void dfs_in_tree(ll start) {
		__dfs_in_tree_impl<G, config>(start, start);
	}
} // namespace mirai