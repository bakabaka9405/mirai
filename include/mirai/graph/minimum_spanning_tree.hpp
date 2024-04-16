#pragma once
#include <mirai/pch.hpp>
#include <mirai/graph/graph.hpp>
#include <mirai/graph/iterate.hpp>
#include <mirai/ds/disjoint_set_union.hpp>
#include <mirai/util/coro.hpp>
MR_NAMESPACE_BEGIN
template <typename T>
	requires(!std::is_same_v<T, void>)
generator<tuple<ll, ll, T>> kruskal(const graph<T>& G, auto&& cmp) {
	disjoint_set_union dsu(G.node_count());
	vector<tuple<ll, ll, T>> edges;
	iterate_all_edges(G) | append_to(edges) | endp;
	ranges::sort(edges, [&](auto&& a, auto&& b) { return cmp(std::get<2>(a), std::get<2>(b)); });
	for (auto&& [u, v, w] : edges) {
		if (dsu.find(u) == dsu.find(v)) continue;
		dsu.merge(u, v);
		co_yield { u, v, w };
	}
}
template <typename T>
	requires(!std::is_same_v<T, void>)
auto kruskal(const graph<T>& G) {
	return kruskal(G, std::less<T>{});
}
MR_NAMESPACE_END