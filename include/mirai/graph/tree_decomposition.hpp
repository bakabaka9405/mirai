#pragma once
#include <mirai/pch.hpp>
#include <mirai/graph/graph.hpp>
#include <mirai/util/pipeline.hpp>
namespace mirai {
	template <auto& G, auto& config>
	inline void __tree_decomposition_impl(ll u, ll top, ll& t) mr_noexcept {
		if constexpr (requires { config.dfn; }) config.dfn[u] = ++t;
		config.top[u] = top;
		if constexpr (requires { config.dfs_order_pt; })
			config.dfs_order[config.dfs_order_pt]++;
		else if constexpr (requires { config.dfs_order; })
			config.dfs_order.push_back(u);
		auto son = G[u] | transform(__edge_get_v) | filter([u](ll v) { return v != config.fa[u]; })
				   | extreme_value<ll>([](ll l, ll r) { return config.size[l] > config.size[r]; }) | endp;
		if (son.has_value()) {
			__tree_decomposition_impl<G, config>(*son, top, t);
			for (auto i : G[u] | transform(__edge_get_v) | filter([u, son](ll v) { return v != *son && v != config.fa[u]; }))
				__tree_decomposition_impl<G, config>(i, i, t);
		}
	}

	template <auto& G, auto& config>
		requires requires {
			config.top[0];
			config.size[0];
			config.fa[0];
		}
	inline void tree_decomposition(ll start) mr_noexcept {
		ll t = 0;
		return __tree_decomposition_impl<G, config>(start, start, t);
	}

	template <auto& config, typename Func>
		requires requires {config.top[0];config.fa[0];config.dep[0]; }
	inline void tree_path_iterate(ll u, ll v, Func&& func) mr_noexcept {
		while (config.top[u] != config.top[v]) {
			if (config.dep[config.top[u]] > config.dep[config.top[v]]) swap(u, v);
			func(u, v);
			v = config.fa[config.top[v]];
		}
		if (config.dep[u] > config.dep[v]) swap(u, v);
		func(u, v);
	}

	template <auto& config>
		requires requires {config.top[0];config.fa[0];config.depth[0]; }
	inline ll tree_path_lca(ll u, ll v) {
		while (config.top[u] != config.top[v]) {
			if (config.dep[config.top[u]] > config.dep[config.top[v]]) swap(u, v);
			v = config.fa[config.top[v]];
		}
		if (config.depth[u] > config.depth[v]) swap(u, v);
		return u;
	}
} // namespace mirai