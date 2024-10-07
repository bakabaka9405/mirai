#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
template <typename T>
class chtholly_tree {
private:
	struct node {
		ll l, r;
		mutable T v;
		bool operator<(const node& rhs) const { return l < rhs.l; }
	};
	std::set<node> s;
	auto split(ll x) {
		auto it = s.lower_bound(node{ x });
		if (it != s.end() && it->l == x) return it;
		--it;
		ll l = it->l, r = it->r;
		T v = it->v;
		s.erase(it);
		s.insert(node{ l, x - 1, v });
		return s.insert(node{ x, r, v }).first;
	}
	auto split(ll l, ll r) {
		auto rt = split(r + 1), lt = split(l);
		return std::make_pair(lt, rt);
	}
	void merge(ll l, ll r, const T& v) {
		auto [rt, lt] = split(l, r);
		s.erase(lt, rt);
		s.insert(node{ l, r, v });
	}
	void merge(ll l, ll r, T&& v) {
		auto [rt, lt] = split(l, r);
		s.erase(lt, rt);
		s.insert(node{ l, r, std::move(v) });
	}

public:
	void insert(ll l, ll r, const T& v) {
		s.insert({ l, r, v });
	}
	void insert(ll l, ll r, T&& v) {
		s.insert({ l, r, std::move(v) });
	}
	void erase(ll l, ll r) {
		auto [rt, lt] = split(l, r);
		s.erase(lt, rt);
	}
	auto extract(ll l, ll r) {
		auto [rt, lt] = split(l, r);
		return ranges::subrange(lt, rt);
	}
};
MR_NAMESPACE_END