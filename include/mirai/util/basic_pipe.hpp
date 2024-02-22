//不知道有什么卵用，先丢在这
#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
template <typename Func, typename... Args>
struct basic_pipe {
	tuple<Args...> args;
	Func func;
	template <typename T>
	friend constexpr inline auto operator|(T&& lhs, basic_pipe<Func, Args...>&& rhs) mr_noexcept->basic_pipe<Func, T, Args...> {
		return { std::tuple_cat(std::forward_as_tuple(lhs), std::move(rhs.args)), std::move(rhs.func) };
	}
	template <typename Callable = Func, typename = std::enable_if_t<std::is_invocable_v<Callable, Args...>>>
	inline constexpr decltype(auto) operator()() const mr_noexcept {
		return std::apply(func, args);
	}
};

template <typename Func>
struct basic_pipe<Func> {
	Func func;
	template <typename T>
	friend inline constexpr auto operator|(T&& lhs, basic_pipe<Func>&& rhs) mr_noexcept->basic_pipe<Func, T> {
		return { std::forward_as_tuple(lhs), std::move(rhs.func) };
	}
	template <typename... Args>
	inline constexpr auto operator()(Args&&... args) const mr_noexcept->basic_pipe<Func, Args...> {
		return { std::forward_as_tuple(std::forward<Args>(args)...), func };
	}
};

template <typename Func>
MR_NODISCARD constexpr auto make_pipe(Func&& func) mr_noexcept->basic_pipe<Func> {
	return { std::forward<Func>(func) };
}
MR_NAMESPACE_END