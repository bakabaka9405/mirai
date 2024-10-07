#pragma once
#include <mirai/pch.hpp>
#include <mirai/graph/graph.hpp>
#include <mirai/graph/iterate.hpp>
#include <mirai/ds/disjoint_set_union.hpp>
#include <mirai/util/coro.hpp>
MR_NAMESPACE_BEGIN
template <typename T, typename Compare = std::less<>>
	requires(!std::is_same_v<T, void>)
generator<tuple<ll, ll, T>> kruskal_coro(const graph<T>& G, Compare&& cmp = Compare{}) {
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
MR_NAMESPACE_END