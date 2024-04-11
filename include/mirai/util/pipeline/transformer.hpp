#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/range.hpp>
MR_NAMESPACE_BEGIN
template <range _range, typename Func>
inline constexpr auto transform(_range&& r, Func&& func) {
	struct transform_wrapper {
		_range _r;
		Func _func;
		struct sentinel {
			decltype(mr_end(_r)) _it;
		};
		struct iterator {
			using iter_t = decltype(mr_begin(_r));
			iter_t _it;
			const Func& _func;
			using value_type = decltype(_func(*_it));
			inline bool operator!=(const sentinel& rt) const mr_noexcept {
				return _it != rt._it;
			}
			inline bool operator==(const sentinel& rt) const mr_noexcept {
				return !this->operator!=(rt);
			}
			inline decltype(auto) operator++() mr_noexcept {
				++_it;
				return *this;
			}
			inline auto operator++(int) mr_noexcept->iterator {
				iterator res = *this;
				this->operator++();
				return res;
			}
			inline auto operator*() const mr_noexcept->value_type {
				return _func(*_it);
			}
		};
		inline auto begin() const mr_noexcept { return iterator{ mr_begin(_r), _func }; }
		inline auto end() const mr_noexcept { return sentinel{ mr_end(_r) }; }
	};
	return transform_wrapper{ std::forward<_range>(r), std::forward<Func>(func) };
}

template <typename Func>
struct __transform_helper { // NOLINT(bugprone-reserved-identifier)
	Func func;
	template <range _range>
	MR_NODISCARD friend constexpr auto operator|(_range&& lhs, __transform_helper&& self) mr_noexcept {
		return transform(std::forward<_range>(lhs), std::move(self.func));
	}
};

template <typename Func>
inline constexpr auto transform(Func&& func) {
	return __transform_helper<Func>{ std::forward<Func>(func) };
}

template <range _range, typename Func>
inline constexpr auto transform_apply(_range&& r, Func&& func) {
	struct transform_apply_wrapper {
		_range _r;
		Func _func;
		struct sentinel {
			decltype(mr_end(_r)) _it;
		};
		struct iterator {
			using iter_t = decltype(mr_begin(_r));
			iter_t _it;
			const Func& _func;
			using value_type = decltype(std::apply(_func, *_it));
			inline bool operator!=(const sentinel& rt) const mr_noexcept {
				return _it != rt._it;
			}
			inline bool operator==(const sentinel& rt) const mr_noexcept {
				return !this->operator!=(rt);
			}
			inline decltype(auto) operator++() mr_noexcept {
				++_it;
				return *this;
			}
			inline auto operator++(int) mr_noexcept->iterator {
				iterator res = *this;
				this->operator++();
				return res;
			}
			inline auto operator*() const mr_noexcept->value_type {
				return std::apply(_func, *_it);
			}
		};
		inline auto begin() const mr_noexcept { return iterator{ mr_begin(_r), _func }; }
		inline auto end() const mr_noexcept { return sentinel{ mr_end(_r) }; }
	};
	return transform_apply_wrapper{ std::forward<_range>(r), std::forward<Func>(func) };
}

template <typename Func>
struct __transform_apply_helper { // NOLINT(bugprone-reserved-identifier)
	Func func;
	template <range _range>
	MR_NODISCARD friend constexpr auto operator|(_range&& lhs, __transform_apply_helper&& self) mr_noexcept {
		return transform(std::forward<_range>(lhs), std::move(self.func));
	}
};

template <typename Func>
inline constexpr auto transform_apply(Func&& func) {
	return __transform_helper<Func>{ std::forward<Func>(func) };
}
template <range range, typename Container>
inline constexpr auto map_to(range&& r, Container&& table) {
	struct maps_to_wrapper {
		using iter_t = decltype(mr_begin(r));
		using sen_t = decltype(mr_end(r));
		range _r;
		Container table;
		struct sentinel {
			sen_t sen;
		};
		struct iterator {
			iter_t it;
			Container& table;
			inline bool operator!=(const sentinel& rt) const { return it != rt.sen; }
			inline decltype(auto) operator*() const { return table[*it]; }
			inline iterator& operator++() {
				++it;
				return *this;
			}
		};
		inline constexpr auto begin() { return iterator{ mr_begin(_r), table }; }
		inline constexpr auto end() const { return sentinel{ mr_end(_r) }; }
	};
	return maps_to_wrapper{ std::forward<range>(r), std::forward<Container>(table) };
}

template <typename Container>
struct __map_to_helper { // NOLINT(bugprone-reserved-identifier)
	Container table;
	template <range range>
	MR_NODISCARD friend inline decltype(auto) operator|(range&& r, __map_to_helper&& self) {
		return map_to(std::forward<range>(r), std::move(self.table));
	}
};

template <typename Container>
inline constexpr auto map_to(Container&& table) {
	return __map_to_helper<Container>{ std::forward<Container>(table) };
}

inline constexpr auto as_abs = [](auto x) { return std::abs(x); };

inline constexpr auto as_square = [](auto x) { return x * x; };

inline constexpr auto is_positive = [](auto x) { return x > 0; };

inline constexpr auto not_negative = [](auto x) { return x >= 0; };

inline constexpr auto to_pair = [](auto&& x) { return pair{ get<0>(x), get<1>(x) }; };

inline constexpr auto equal_to = [](auto&& x) {
	return [x](auto&& y) {
		return x == y;
	};
};

inline constexpr auto not_equal_to = [](auto&& x) {
	return [x](auto&& y) {
		return x != y;
	};
};

template <typename Dst>
inline constexpr auto to_struct = [](auto&& x) { return std::apply([&](auto&&... args) -> Dst { return { args... }; }, std::forward<std::decay_t<decltype(x)>>(x)); };

MR_NAMESPACE_END