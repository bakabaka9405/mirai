#pragma once
#include <mirai/pch.hpp>
namespace mirai {
	namespace detail {
		template <typename T>
		class persistent_stack_node {
		public:
			const persistent_stack_node* fa;
			T val;
			persistent_stack_node(const persistent_stack_node* fa, const T& val) mr_noexcept
				: fa(fa),
				  val(val) {}
		};
	} // namespace detail
	template <typename T, typename Node = detail::persistent_stack_node<T>, typename Allocator = std::pmr::polymorphic_allocator<Node>>
	class persistent_stack {
	private:
		Allocator alloc;
		const Node* node;
		size_t dep;
		const Node* construct(const Node* fa, const T& val) mr_noexcept {
			if constexpr (requires { alloc.new_object; }) {
				return alloc.template new_object<Node>(fa, val);
			}
			else {
				Node* x = alloc.allocate(1);
				alloc.construct(x, fa, val);
				return x;
			}
		}
		persistent_stack(const Node* fa, const T& val, size_t dep, Allocator alloc) mr_noexcept
			: alloc(alloc),
			  node(construct(fa, val)),
			  dep(dep) {}
		persistent_stack(const Node* node, size_t dep, Allocator alloc) mr_noexcept
			: alloc(alloc),
			  node(node),
			  dep(dep) {}

	public:
		persistent_stack(Allocator alloc = {}) mr_noexcept
			: alloc(alloc),
			  node(construct(nullptr, T{ 114514 })),
			  dep{ 0 } {}
		persistent_stack(const persistent_stack& other) mr_noexcept
			: alloc(other.alloc),
			  node(other.node),
			  dep(other.dep) {}
		MR_NODISCARD persistent_stack push(const T& val) mr_noexcept { return persistent_stack(node, val, dep + 1, alloc); }
		MR_NODISCARD persistent_stack pop() const mr_noexcept { return persistent_stack(node->fa, dep - 1, alloc); }
		MR_NODISCARD T top() const mr_noexcept { return node->val; }
		MR_NODISCARD bool empty() const mr_noexcept { return !dep; }
		MR_NODISCARD size_t size() const mr_noexcept { return dep; }
	};
} // namespace mirai