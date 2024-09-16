#pragma once
#include <mirai/pch.hpp>
#include <mirai/graph/graph.hpp>
MR_NAMESPACE_BEGIN
template <typename T, typename Compare = std::less<>>
void dijkstra(graph<T>&& G, std::random_access_iterator auto dis, ll start, auto&& get_weight, Compare&& cmp = Compare{}) {
	vector<uint8_t> vis(G.node_count());
	using dis_t = std::conditional_t<std::is_same_v<T, void>, ll, T>;
	using node = pair<dis_t, ll>;
	priority_queue<node, vector<node>, std::greater<>> pq;
	pq.emplace(0, start);
	dis[start] = 0;
	while (!pq.empty()) {
		auto u = pq.top().second;
		pq.pop();
		if (vis[u]) continue;
		vis[u] = 1;
		for (auto&& [v, data] : G[u]) {
			dis_t w;
			if constexpr (std::is_same_v<T, void>) w = 1;
			else w = get_weight(data);
			w = get_weight(data);
			if (cmp(dis[u] + w, dis[v])) {
				dis[v] = dis[u] + w;
				if (!vis[v]) pq.emplace(dis[v], v);
			}
		}
	}
}
template <typename T, typename Compare = std::less<>>
void spfa(graph<T>&& G, std::random_access_iterator auto dis, ll start, auto&& get_weight, Compare&& cmp = Compare{}) {
	using dis_t = std::conditional_t<std::is_same_v<T, void>, ll, T>;
	vector<uint8_t> in_queue(G.node_count, 1);
	queue<ll> q;
	q.push(start);
	in_queue[start] = 1;
	while (!q.empty()) {
		auto u = q.front();
		q.pop();
		in_queue[u] = 0;
		for (auto&& [v, data] : G[u]) {
			dis_t w;
			if constexpr (std::is_same_v<T, void>) w = 1;
			else w = get_weight(data);
			if (cmp(dis[u] + w, dis[v])) {
				dis[v] = dis[u] + w;
				if (!in_queue[v]) q.emplace(dis[v], v), in_queue[v] = 1;
			}
		}
	}
}
MR_NAMESPACE_END