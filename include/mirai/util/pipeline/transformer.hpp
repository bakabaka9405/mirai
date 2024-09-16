#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/range.hpp>
MR_NAMESPACE_BEGIN

class __transform_fn {
private:
	template <size_t I = 0, typename T, typename... Func>
	inline constexpr static auto link_invoke(tuple<Func...>& func, T&& arg) mr_noexcept {
		if constexpr (I == sizeof...(Func)) {
			return arg;
		}
		else {
			return link_invoke<I + 1>(func, std::invoke(get<I>(func), std::forward<T>(arg)));
		}
	}

	template <size_t I = 0, typename T, typename... Func>
	inline constexpr static auto link_invoke_const(const tuple<Func...>& func, T&& arg) mr_noexcept {
		if constexpr (I == sizeof...(Func)) {
			return arg;
		}
		else {
			return link_invoke_const<I + 1>(func, std::invoke(get<I>(func), std::forward<T>(arg)));
		}
	}
	template <range _range, typename... Func>
	inline constexpr static auto invoke(_range&& r, tuple<Func...>&& func) mr_noexcept {
		struct transform_wrapper {
			_range _r;
			tuple<Func...> _func;
			struct sentinel {
				using sentinel_t = std::decay_t<decltype(mr_end(_r))>;
				sentinel_t _sen;
			};
			struct iterator {
				using iter_t = std::decay_t<decltype(mr_begin(_r))>;
				iter_t _it;
				tuple<Func...>& _func;
				using value_type = decltype(__transform_fn::link_invoke(_func, *_it));
				inline bool operator!=(const sentinel& rt) const mr_noexcept {
					return _it != rt._sen;
				}
				inline bool operator==(const sentinel& rt) const mr_noexcept {
					return !this->operator!=(rt);
				}
				inline decltype(auto) operator++() mr_noexcept {
					++_it;
					return *this;
				}
				inline auto operator++(int) mr_noexcept->iterator {
					auto res = *this;
					this->operator++();
					return res;
				}
				inline auto operator*() mr_noexcept->value_type {
					return __transform_fn::link_invoke(_func, *_it);
				}
			};
			struct const_iterator {
				using iter_t = std::decay_t<decltype(mr_begin(_r))>;
				iter_t _it;
				const tuple<Func...>& _func;
				using value_type = decltype(__transform_fn::link_invoke_const(_func, *_it));
				inline bool operator!=(const sentinel& rt) const mr_noexcept {
					return _it != rt._sen;
				}
				inline bool operator==(const sentinel& rt) const mr_noexcept {
					return !this->operator!=(rt);
				}
				inline decltype(auto) operator++() mr_noexcept {
					++_it;
					return *this;
				}
				inline auto operator++(int) mr_noexcept->const_iterator {
					auto res = *this;
					this->operator++();
					return res;
				}
				inline auto operator*() mr_noexcept->value_type {
					return __transform_fn::link_invoke_const(_func, *_it);
				}
			};

			inline auto begin() mr_noexcept { return iterator{ mr_begin(_r), _func }; }
			inline auto begin() const mr_noexcept { return const_iterator{ mr_begin(_r), _func }; }
			inline auto end() const mr_noexcept {
				return sentinel{ mr_end(_r) };
			}
		};
		return transform_wrapper{ std::forward<_range>(r), std::forward<tuple<Func...>>(func) };
	}

	template <typename... Func>
	struct __transform_helper {
		friend class __transform_fn;
		tuple<Func...> func;
		template <range _range>
		constexpr friend auto operator|(_range&& r, __transform_helper&& self) mr_noexcept {
			return __transform_fn::invoke(std::forward<_range>(r), std::move(self.func));
		}
		template <range _range>
		constexpr friend auto operator|(_range&& r, const __transform_helper& self) mr_noexcept {
			return __transform_fn::invoke(std::forward<_range>(r), tuple<Func...>{ self.func });
		}
	};

public:
	template <typename... Func>
	constexpr auto operator()(Func&&... func) const mr_noexcept {
		return __transform_helper<Func...>{ std::forward_as_tuple(std::forward<Func>(func)...) };
	}
};

constexpr __transform_fn transform;

template <typename Func>
inline constexpr auto transform_apply(Func&& func) {
	return transform([func = std::forward<Func>(func)](auto&& v) { return std::apply(func, v); });
}
MR_NAMESPACE_END