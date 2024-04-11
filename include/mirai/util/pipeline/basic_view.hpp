// 不知道有什么卵用，先丢在这
#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/range.hpp>
MR_NAMESPACE_BEGIN
template <typename Func, typename... Args>
struct basic_view {
	tuple<Args...> args;
	Func func;
	template <range _range, typename Callable = Func, typename = std::enable_if_t<std::is_invocable_v<Callable, _range, Args...>>>
	friend inline constexpr auto operator|(_range&& lhs, basic_view<Func, Args...>&& rhs) mr_noexcept {
		return std::apply(rhs.func, tuple_cat(std::forward_as_tuple(lhs), std::move(rhs.args)));
	}
};

template <typename Func>
struct basic_view<Func> {
	Func func;
	template <typename... Args>
	inline constexpr auto operator()(Args&&... args) const mr_noexcept->basic_view<Func, Args...> {
		return { std::forward_as_tuple(std::forward<Args>(args)...), func };
	}
};

template <typename Func>
MR_NODISCARD constexpr auto make_view(Func&& func) mr_noexcept->basic_view<Func> {
	return { std::forward<Func>(func) };
}
MR_NAMESPACE_END