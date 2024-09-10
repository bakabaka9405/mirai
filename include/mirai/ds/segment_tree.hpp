#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
template <typename T, typename Container = std::vector<T>>
	requires requires(T&& x, T&& y) { {x+y}->std::same_as<T>;T{};T{-1}; }
class segment_tree {
public:
	using value_type = T;
	using size_type = std::size_t;
	using container_type = Container;
	static constexpr T e = T{};
	static constexpr T o = T{ -1 };
	void build_impl(size_type p, size_type l, size_type r, auto&& container) mr_noexcept {
		if (l == r) {
			data[p] = container[l];
			return;
		}
		size_type m = (l + r) >> 1u;
		build_impl(p << 1u, l, m, container);
		build_impl(p << 1u | 1u, m + 1, r, container);
		data[p] = data[p << 1u] + data[p << 1u | 1u];
	}
	void update_impl(size_type p, size_type l, size_type r, size_type x, const T& v) mr_noexcept {
		if (l == r) {
			data[p] = v;
			return;
		}
		size_type m = (l + r) >> 1u;
		if (x <= m) update_impl(p << 1u, l, m, x, v);
		else update_impl(p << 1u | 1u, m + 1, r, x, v);
		data[p] = data[p << 1u] + data[p << 1u | 1u];
	}

	T query_impl(size_type p, size_type l, size_type r, size_type ql, size_type qr) const mr_noexcept {
		if (ql <= l && r <= qr) {
			return data[p];
		}
		size_type m = (l + r) >> 1u;
		if (qr <= m) return query_impl(p << 1u, l, m, ql, qr);
		if (ql > m) return query_impl(p << 1u | 1u, m + 1, r, ql, qr);
		return query_impl(p << 1u, l, m, ql, qr) + query_impl(p << 1u | 1u, m + 1, r, ql, qr);
	}

private:
	container_type data;
	size_type n;

public:
	explicit segment_tree(size_type n) mr_noexcept
		: n(n),
		  data(n << 2u, e) {}
	segment_tree(size_type n, auto&& container) mr_noexcept
		: n(n) {
		build(n, container);
	}
	void build(size_type n) mr_noexcept {
		this->n = n;
		data.resize(n << 2u, e);
	}
	void build(size_type n, auto&& container) mr_noexcept {
		this->n = n;
		data.resize(n << 2u, e);
		build_impl(1, 0, n - 1, container);
	}
	void update(size_type x, const T& v) mr_noexcept {
		update_impl(1, 0, n - 1, x, v);
	}
	T query(size_type l, size_type r) const mr_noexcept {
		if (l > r) return o;
		return query_impl(1, 0, n - 1, l, r);
	}
};
MR_NAMESPACE_END