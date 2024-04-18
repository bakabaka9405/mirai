#pragma once
#include <mirai/pch.hpp>
#include<mirai/util/concepts.hpp>
MR_NAMESPACE_BEGIN
enum class sam_state_type {
	array,
	map
};
namespace detail {
	template <size_t N>
	struct array_sam_state {
		array<array_sam_state*, N> next;
		array_sam_state* link;
		size_t len;
		array_sam_state()
			: next(), link(nullptr), len(0) {}

		auto operator[](size_t i) mr_noexcept->array_sam_state*& { return next[i]; }
		auto operator[](size_t i) const mr_noexcept->const array_sam_state*& { return next[i]; }
		auto get(size_t i) mr_noexcept->array_sam_state*& { return next[i]; }
		auto get(size_t i) const mr_noexcept->const array_sam_state*& { return next[i]; }
		void set(size_t i, array_sam_state* v) mr_noexcept { next[i] = v; }
	};
	// template <typename Map>
	struct map_sam_state {
		map<ull, map_sam_state*> next;
		map_sam_state* link;
		size_t len;
		map_sam_state()
			: next(), link(nullptr), len(0) {}
		auto operator[](size_t i) const mr_noexcept->map_sam_state* const& { return next.at(i); }
		auto get(size_t i) const mr_noexcept->map_sam_state* const& { return next.at(i); }
		void set(size_t i, map_sam_state* const v) mr_noexcept { next[i] = v; }
	};

	template <typename StateType, typename Allocator>
	class basic_sam {
	private:
		StateType* root;
		StateType* last;
		Allocator allocator;

	public:
		constexpr explicit basic_sam(Allocator const& allocator = Allocator()) mr_noexcept
			: root(allocator.allocate(1)),
			  last(root),
			  allocator(allocator) {
			new (root) StateType();
		}
		~basic_sam() {
			allocator.deallocate(root, 1);
		}
		void insert(size_t c) mr_noexcept {
			StateType* p = last;
			StateType* np = allocator.allocate(1);
			new (np) StateType();
			np->len = p->len + 1;
			for (; MR_EXPECT(p && !p->get(c)); p = p->link) p->set(c, np);
			if (!p) {
				np->link = root;
			}
			else {
				StateType* q = p->get(c);
				if (q->len == p->len + 1) {
					np->link = q;
				}
				else {
					StateType* nq = allocator.allocate(1);
					new (nq) StateType();
					nq->len = p->len + 1;
					nq->link = q->link;
					nq->next = q->next;
					for (; MR_EXPECT(p && p->get(c) == q); p = p->link) {
						p->set(c, nq);
					}
					q->link = np->link = nq;
				}
			}
			last = np;
		}

		template <typename InputIt>
		void insert(InputIt first, InputIt last) mr_noexcept {
			for (; first != last; ++first) insert(*first);
		}

		template <typename Container>
		void insert(Container const& c) mr_noexcept {
			insert(mr_begin(c), mr_end(c));
		}

	};
} // namespace detail

template <size_t N, typename Allocator = std::allocator<detail::array_sam_state<N>>>
using array_suffix_automaton = detail::basic_sam<detail::array_sam_state<N>, Allocator>;

template <typename Map, typename Allocator = std::allocator<detail::map_sam_state>>
using map_suffix_automaton = detail::basic_sam<detail::map_sam_state, Allocator>;

namespace pmr {
	template <size_t N>
	using array_suffix_automaton = mirai::array_suffix_automaton<N, std::pmr::polymorphic_allocator<detail::array_sam_state<N>>>;
	using map_suffix_automaton = mirai::map_suffix_automaton<std::pmr::polymorphic_allocator<detail::map_sam_state>>;
} // namespace pmr
MR_NAMESPACE_END