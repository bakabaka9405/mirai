#pragma once
#include <mirai/pch.hpp>
#include <mirai/graph/graph.hpp>
MR_NAMESPACE_BEGIN
namespace details {
	struct flow_edge {
		ll v, cap, cost;
	};
} // namespace details
class flow : public graph<ll, link_model> {
private:
	using graph::insert;
	using graph::insert_bothway;

public:
	flow() mr_noexcept
		: graph() {}
	explicit flow(size_t n) mr_noexcept
		: graph(n) {}
	void insert(ll u, ll v, ll cap) mr_noexcept {
		graph::insert(u, v, cap);
		graph::insert(v, u, 0);
	}
	void reset()mr_noexcept{

	}
};
MR_NAMESPACE_END