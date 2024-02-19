#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
template <typename T>
struct segment_tree_node {
	T val;
	segment_tree_node* left;
	segment_tree_node* right;
	explicit segment_tree_node(const T& val, segment_tree_node* left = nullptr, segment_tree_node* right = nullptr) mr_noexcept
		: val(val),
		  left(left),
		  right(right) {}
	explicit segment_tree_node(T&& val, segment_tree_node* left = nullptr, segment_tree_node* right = nullptr) mr_noexcept
		: val(std::move(val)),
		  left(left),
		  right(right) {}
};
template <typename T, size_t size, typename Node = segment_tree_node<T>, typename Allocator = std::pmr::polymorphic_allocator<Node>>
class persistent_array {
private:
	Allocator alloc;
	Node* root;
	Node* construct(auto&& val, Node* left = nullptr, Node* right = nullptr) mr_noexcept {
		if constexpr (requires { alloc.new_object; }) {
			return alloc.template new_object<Node>(std::forward<T>(val), left, right);
		}
		else {
			Node* x = alloc.allocate(1);
			alloc.construct(x, std::forward<T>(val), left, right);
			return x;
		}
	}
	const T& visit(ull pos) const mr_noexcept {
		Node* x = root;
		ull L = 0, R = size;
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
	T& visit(ull pos) mr_noexcept {
		root = construct(root->val, root->left, root->right);
		Node* x = root;
		ull L = 0, R = size;
		while (L != R) {
			MR_ASSUME(x != nullptr);
			ull M = (L + R) / 2;
			if (pos <= M) {
				x = x->left = construct(x->left->val, x->left->left, x->left->right);
				R = M;
			}
			else {
				x = x->right = construct(x->right->val, x->right->left, x->right->right);
				L = M + 1;
			}
		}
		return x->val;
	}
	Node* set(Node* x, ull pos, auto&& val, ull L, ull R) mr_noexcept {
		if (L == R) return construct(std::forward<T>(val));
		ull M = (L + R) / 2;
		MR_ASSUME(x != nullptr);
		if (pos <= M)
			return construct(x->val, set(x->left, pos, std::forward<T>(val), L, M), x->right);
		else
			return construct(x->val, x->left, set(x->right, pos, std::forward<T>(val), M + 1, R));
	}
	template <size_t N>
	Node* build(T (&arr)[N], ull L, ull R) {
		if (L == R) {
			if (L < N)
				return construct(arr[L]);
			else
				return construct(T{}); // default value
		}
		ull M = (L + R) / 2;
		return construct(0, build(arr, L, M), build(arr, M + 1, R));
	}
	explicit persistent_array(Node* root, Allocator alloc = {})
		: alloc(alloc), root(root) {}

public:
	persistent_array() mr_noexcept
		: alloc{},
		  root(nullptr) {}
	explicit persistent_array(std::pmr::monotonic_buffer_resource* pool) mr_noexcept
		: alloc{ pool },
		  root(nullptr) {}
	template <size_t N>
	persistent_array(T (&val)[N], std::pmr::monotonic_buffer_resource* pool) mr_noexcept
		: alloc(pool),
		  root(build(val, 0, size)) {}
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
		root = build(val, 0, size);
		return *this;
	}
	persistent_array set_copy(ull pos, auto&& val) mr_noexcept {
		return persistent_array(set(root, pos, std::forward<T>(val), 0, size), alloc);
	}
	const T& operator[](ull pos) const mr_noexcept {
		return visit(pos);
	}
	T& operator[](ull pos) mr_noexcept {
		return visit(pos);
	}
};
MR_NAMESPACE_END