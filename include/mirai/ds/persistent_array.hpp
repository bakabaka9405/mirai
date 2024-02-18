#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
template <typename T>
struct segment_tree_node {
	T val;
	segment_tree_node* left;
	segment_tree_node* right;
	explicit segment_tree_node(T val, segment_tree_node* left = nullptr, segment_tree_node* right = nullptr) mr_noexcept
		: val(val),
		  left(left),
		  right(right) {}
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
	const T& get(Node* x, ull pos, ull L, ull R) const mr_noexcept {
		while (L != R) {
			MR_ASSUME(x != nullptr);
			ull M = (L + R) / 2;
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
	pair<Node*, T&> get(Node* x, ull pos, ull L, ull R) mr_noexcept {
		MR_ASSUME(x != nullptr);
		if (L == R) {
			auto obj = construct(x->val);
			return { obj, obj->val };
		}
		ull M = (L + R) / 2;
		if (pos <= M) {
			auto [left, ref] = get(x->left, pos, L, M);
			return { construct(x->val, left, x->right), ref };
		}
		else {
			auto [right, ref] = get(x->right, pos, M + 1, R);
			return { construct(x->val, x->left, right), ref };
		}
	}
	Node* set(Node* x, ull pos, T val, ull L, ull R) mr_noexcept {
		if (L == R) return construct(val);
		ull M = (L + R) / 2;
		MR_ASSUME(x != nullptr);
		if (pos <= M)
			return construct(x->val, set(x->left, pos, val, L, M), x->right);
		else
			return construct(x->val, x->left, set(x->right, pos, val, M + 1, R));
	}
	template <size_t N>
	Node* set(T (&arr)[N], ull L, ull R) {
		if (L == R) {
			if (L < N)
				return construct(arr[L]);
			else
				return construct(T{}); // default value
		}
		ull M = (L + R) / 2;
		return construct(0, set(arr, L, M), set(arr, M + 1, R));
	}
	explicit persistent_array(Node* root, Allocator alloc = {})
		: alloc(alloc), root(root) {}

public:
	persistent_array() mr_noexcept
		: alloc{}, root(nullptr) {}
	explicit persistent_array(std::pmr::monotonic_buffer_resource* pool) mr_noexcept
		: alloc{ pool }, root(nullptr) {}
	template <size_t N>
	persistent_array(T (&val)[N], std::pmr::monotonic_buffer_resource* pool) mr_noexcept
		: alloc(pool), root(set(val, 0, size)) {}
	persistent_array(const persistent_array& x)
		: alloc{ x.alloc }, root(x.root) {}
	persistent_array(persistent_array&& x) noexcept
		: alloc{ std::move(x.alloc) }, root(x.root) {
		x.root = nullptr;
	}
	persistent_array& operator=(const persistent_array& x) mr_noexcept {
		if (this != &x) {
			root = x.root;
		}
		return *this;
	}
	template <size_t N>
	persistent_array& operator=(T (&val)[N]) mr_noexcept {
		root = set(val, 0, size);
		return *this;
	}
	const T& operator[](ull pos) const mr_noexcept {
		return get(root, pos, 0, size);
	}
	T& operator[](ull pos) mr_noexcept {
		auto&& [new_root, ref] = get(root, pos, 0, size);
		root = new_root;
		return ref;
	}
};
MR_NAMESPACE_END