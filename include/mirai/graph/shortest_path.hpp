#pragma once
#include <mirai/pch.hpp>
#include <mirai/graph/graph.hpp>
MR_NAMESPACE_BEGIN
void dijkstra(weighted_graph auto&& G, std::random_access_iterator auto dis, ll start, auto&& get_weight) {
	vector<int8_t> vis(G.node_count(), static_cast<int8_t>(0));
	using node = pair<ll, ll>;
	priority_queue<node, vector<node>, std::greater<>> pq;
	pq.emplace(0, start);
	dis[start] = 0;
	while (!pq.empty()) {
		auto u = pq.top().second;
		pq.pop();
		if (vis[u]) continue;
		vis[u] = 1;
		for (auto&& [v, data] : G[u]) {
			auto&& w = get_weight(data);
			if (dis[u] + w < dis[v]) {
				dis[v] = dis[u] + w;
				if (!vis[v]) pq.push({ dis[v], v });
			}
		}
	}
}
void spfa(weighted_graph auto&& G, std::random_access_iterator auto dis, ll start, auto&& get_weight, auto&& cmp = std::less<ll>()) {
	vector<int8_t> in_queue(G.node_count, 1);
	queue<ll> q;
	q.push(start);
	in_queue[start] = 1;
	while (!q.empty()) {
		auto u = q.front();
		q.pop();
		in_queue[u] = 0;
		for (auto&& [v, data] : G[u]) {
			auto&& w = get_weight(data);
			if (cmp(dis[u] + w, dis[v])) {
				dis[v] = dis[u] + w;
				if (!in_queue[v]) q.push({ dis[v], v }), in_queue[v] = 1;
			}
		}
	}
}
MR_NAMESPACE_END