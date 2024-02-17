#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
template <typename T>
struct segment_tree_node {
	T val;
	segment_tree_node* left;
	segment_tree_node* right;
	segment_tree_node(T val, segment_tree_node* left = nullptr, segment_tree_node* right = nullptr)
		: val(val), left(left), right(right) {}
};
template <typename T, size_t size, typename Node = segment_tree_node<T>, typename Allocator = std::pmr::polymorphic_allocator<Node>>
class persistent_array {
private:
	Allocator alloc;
	Node* root;
	Node* construct(T val, Node* left = nullptr, Node* right = nullptr) {
		if constexpr (requires { alloc.new_object; }) {
			return alloc.template new_object<Node>(val, left, right);
		}
		else {
			Node* x = alloc.allocate(1);
			alloc.construct(x, val, left, right);
			return x;
		}
	}
	T get(Node* x, ll pos, ll L, ll R) {
		while (L != R) {
			ll M = (L + R) / 2;
			if (pos <= M) {
				x = x->left;
				R = M;
			}
			else {
				x = x->right;
				L = M + 1;
			}
		}
		return x->val;
	}
	Node* set(Node* x, ll pos, T val, ll L, ll R) {
		if (L == R) return construct(val);
		ll M = (L + R) / 2;
		if (pos <= M)
			return construct(x->val, set(x->left, pos, val, L, M), x->right);
		else
			return construct(x->val, x->left, set(x->right, pos, val, M + 1, R));
	}
	template <size_t N>
	Node* set(ll (&arr)[N], ll L, ll R) {
		if (L == R) return construct(arr[L]);
		ll M = (L + R) / 2;
		return construct(0, set(arr, L, M), set(arr, M + 1, R));
	}
	persistent_array(Node* root, Allocator alloc = {})
		: alloc(alloc), root(root) {}

public:
	persistent_array()
		: alloc{}, root(nullptr) {}
	persistent_array(std::pmr::monotonic_buffer_resource* pool)
		: alloc{ pool }, root(nullptr) {}
	persistent_array(const persistent_array& x)
		: alloc{ x.alloc }, root(x.root) {}
	persistent_array(persistent_array&& x)
		: alloc{ std::move(x.alloc) }, root(x.root) {
		x.root = nullptr;
	}
	template <size_t N>
	persistent_array(T (&val)[N], std::pmr::monotonic_buffer_resource* pool)
		: alloc(pool), root(set(val, 0, size)) {}
	T get(ll i) {
		return get(root, i, 0, size);
	}
	persistent_array set(ll i, T val) {
		return persistent_array{ set(root, i, val, 0, size), alloc };
	}
};
MR_NAMESPACE_END