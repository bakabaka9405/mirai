#pragma once
#include <mirai/pch.hpp>
#include <mirai/graph/graph.hpp>
#include <mirai/util/pipeline.hpp>
MR_NAMESPACE_BEGIN
template <auto& G, auto& config>
inline void __tree_decomposition_impl(ll u, ll top, ll& t) mr_noexcept {
	if constexpr (requires { config.dfn; }) config.dfn[u] = ++t;
	config.top[u] = top;
	if constexpr (requires { config.dfs_order_pt; })
		config.dfs_order[config.dfs_order_pt]++;
	else if constexpr (requires { config.dfs_order; })
		config.dfs_order.push_back(u);
	ll son = std::numeric_limits<ll>::min();
	for (ll v : G[u])
		if (v != config.fa[u] && (son == std::numeric_limits<ll>::min() || config.size[v] > config.size[son])) son = v;
	if (son == std::numeric_limits<ll>::min()) return;
	__tree_decomposition_impl<G, config>(son, top, t);
	for (ll v : G[u])
		if (v != config.fa[u] && v != son) __tree_decomposition_impl<G, config>(v, v, t);
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
MR_NAMESPACE_END