#pragma once
#include <mirai/pch.hpp>
#include <mirai/graph/graph.hpp>
namespace mirai {
	void dijkstra(auto&& G, auto& dis, ll start, auto&& get_weight) {
		vector<ll> vis(G.node_count, 0);
		using node = pair<ll, ll>;
		priority_queue<node, vector<node>, std::greater<node>> pq;
		pq.push({ 0, start });
		while (!pq.empty()) {
			auto u = pq.top().second;
			pq.pop();
			if (vis[u]) continue;
			vis[u] = 1;
			for (auto&& [v, data] : G[u]) {
				auto w = get_weight(data);
				if (dis[u] + w < dis[v]) {
					dis[v] = dis[u] + w;
					if (!vis[v]) pq.push({ dis[v], v });
				}
			}
		}
	}
	void spfa(auto&& G, auto& dis, ll start, auto&& get_weight) {
		vector<ll> in_queue(G.node_count, 1);
		using node = pair<ll, ll>;
		queue<ll> q;
		q.push(start);
		in_queue[start] = 1;
		while (!q.empty()) {
			auto u = q.front();
			q.pop();
			in_queue[u] = 0;
			for (auto&& [v, data] : G[u]) {
				auto w = get_weight(data);
				if (dis[u] + w < dis[v]) {
					dis[v] = dis[u] + w;
					if (!in_queue[v]) q.push({ dis[v], v }), in_queue[v] = 1;
				}
			}
		}
	}
} // namespace mirai