#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/range.hpp>
MR_NAMESPACE_BEGIN
template <range _range, typename Func>
inline auto filter(_range&& r, Func&& func) {
	struct filter_wrapper {
		_range _r;
		Func _func;
		using sentinel_t = std::decay_t<decltype(mr_end(_r))>;
		struct iterator {
			using iter_t = std::decay_t<decltype(mr_begin(_r))>;
			iter_t _it;
			const Func& _func_ref;
			const sentinel_t _end;
			using value_type = decltype(*_it);
			inline bool operator!=(const sentinel_t& rt) const mr_noexcept {
				return _it != rt;
			}
			inline bool operator==(const sentinel_t& rt) const mr_noexcept {
				return !this->operator!=(rt);
			}
			inline decltype(auto) operator++() mr_noexcept {
				do {
					++_it;
				} while (_it != _end && !_func_ref(*_it));
				return *this;
			}
			inline auto operator++(int) mr_noexcept->iterator {
				iterator it = *this;
				this->operator++();
				return it;
			}
			inline auto operator*() const mr_noexcept->value_type {
				return *_it;
			}
		};
		inline auto begin() const mr_noexcept {
			auto it = mr_begin(_r);
			auto end = mr_end(_r);
			while (it != end && !_func(*it)) {
				++it;
			}
			return iterator{ it, _func, end };
		}
		inline auto end() const mr_noexcept { return mr_end(_r); }
	};
	return filter_wrapper{ std::forward<_range>(r), std::forward<Func>(func) };
};
template <typename Func>
struct __filter_helper { 
	Func func;

	template <range _range>
	MR_NODISCARD friend inline auto operator|(_range&& lhs, __filter_helper&& self) mr_noexcept {
		return filter(std::forward<_range>(lhs), std::move(self.func));
	}
};

template <typename Func>
inline auto filter(Func&& func) {
	return __filter_helper{ std::forward<Func>(func) };
}

inline constexpr auto is_positive = [](auto x) { return x > 0; };

inline constexpr auto not_negative = [](auto x) { return x >= 0; };

template <typename T>
inline constexpr auto equal_to(T&& x) mr_noexcept {
	struct neq_wrapper {
		T val;
		constexpr bool operator()(const T& y) const mr_noexcept {
			return val == y;
		}
	};
	return neq_wrapper{ std::forward<T>(x) };
};

template <typename T>
inline constexpr auto not_equal_to(T&& x) mr_noexcept {
	struct neq_wrapper {
		T val;
		constexpr bool operator()(const T& y) const mr_noexcept {
			return val != y;
		}
	};
	return neq_wrapper{ std::forward<T>(x) };
};
MR_NAMESPACE_END