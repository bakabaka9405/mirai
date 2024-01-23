#pragma once
#include <mirai/pch.hpp>
#include <mirai/graph/graph.hpp>
namespace mirai {
	template <auto& config>
		requires requires { config.G; }
	void __dfs_in_graph_impl(ll u, ll fa) {
		if constexpr (requires { config.fa; }) config.fa[u] = fa;
		if constexpr (requires { config.dfn;config.timestamp; }) config.dfn[u] = ++config.timestamp;
		if constexpr (requires { config.size; }) config.size[u] = 1;
		for (auto&& [v, w] : config.G[u]) {
			[[unlikely]] if (v == fa)
				continue;
			__dfs_in_graph_impl<config>(v, u);
			if constexpr (requires { config.size; }) config.size[u] += config.size[v];
		}
		if constexpr (requires { config.dfn;config.lev;config.timestamp; }) config.lev[u] = config.timestamp;
	}

	template <auto& config>
	void dfs_in_graph(ll start) {
		__dfs_in_graph_impl<config>(start, start);
	}
} // namespace mirai