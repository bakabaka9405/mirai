#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/range.hpp>
MR_NAMESPACE_BEGIN

template <range _range, typename... Func>
inline constexpr auto dispatch(_range&& r, Func&&... func) {
	struct dispatch_wrapper {
		_range _r;
		tuple<Func...> _func;
		inline void operator()() const mr_noexcept {
			for (auto&& i : _r)
				std::apply([&]<typename... Args>(Args&&... f) -> void { ((f(std::forward<decltype((i))>(i))), ...); }, _func);
		}
	};
	return dispatch_wrapper{ std::forward<_range>(r), std::forward_as_tuple(std::forward<Func>(func)...) };
}

template <typename... Func>
struct __dispatch_helper { 
	tuple<Func...> func;
	template <range _range>
	MR_NODISCARD friend constexpr inline decltype(auto) operator|(_range&& lhs, __dispatch_helper&& self) mr_noexcept {
		return std::apply([&](Func&&... f) { return dispatch(std::forward<_range>(lhs), std::forward<Func>(f)...); }, std::move(self.func));
	}
};

template <typename... Func>
	requires(!range<Func> && ...)
MR_NODISCARD inline constexpr auto dispatch(Func&&... func) mr_noexcept {
	return __dispatch_helper<Func...>{ std::forward_as_tuple(std::forward<Func>(func)...) };
}

template <range _range, typename T>
MR_NODISCARD inline auto addup_to(_range&& r, T& dst) {
	struct addup_to {
		_range _r;
		T& _dst;
		inline auto operator()() const mr_noexcept {
			for (auto&& i : _r) _dst += i;
		}
	};
	return addup_to{ std::forward<_range>(r), dst };
}

template <typename T>
struct __addup_to_helper { 
	T& _dst;
	template <range _range>
	MR_NODISCARD friend inline auto operator|(_range&& lhs, __addup_to_helper rhs) mr_noexcept {
		return addup_to(std::forward<_range>(lhs), rhs._dst);
	}
};

template <typename T>
MR_NODISCARD inline auto addup_to(T& dst) {
	return __addup_to_helper{ dst };
}

template <range _range, typename T>
MR_NODISCARD inline auto save_to(_range&& r, T&& dst) {
	struct save_to_wrapper {
		_range _r;
		T _dst;
		inline auto operator()() mr_noexcept {
			for (auto&& i : _r) *_dst++ = i;
		}
	};
	return save_to_wrapper{ std::forward<_range>(r), std::forward<T>(dst) };
}

template <range range_l, range range_r>
MR_NODISCARD inline auto save_to(range_l&& r, range_r&& dst) {
	return save_to(std::forward<range_l>(r), mr_begin(dst));
}

template <typename T>
struct __save_to_helper { 
	T _dst;
	template <range _range>
	MR_NODISCARD friend inline auto operator|(_range&& lhs, __save_to_helper rhs) mr_noexcept {
		return save_to(std::forward<_range>(lhs), std::move(rhs._dst));
	}
};

template <typename T>
MR_NODISCARD inline auto save_to(T&& dst) {
	return __save_to_helper{ std::forward<T>(dst) };
}

template <range range>
MR_NODISCARD inline auto save_to(range&& dst) {
	return __save_to_helper{ mr_begin(dst) };
}

template <range _range, typename Container>
MR_NODISCARD inline auto append_to(_range&& r, Container& dst) {
	struct append_to_wrapper {
		_range _r;
		Container& dst;
		inline auto operator()() mr_noexcept {
			auto it = std::inserter(dst, mr_end(dst));
			for (auto&& i : _r) *it++ = i;
		}
	};
	return append_to_wrapper{ std::forward<_range>(r), dst };
}

template <typename Container>
struct __append_to_helper { 
	Container& _dst;
	template <range _range>
	MR_NODISCARD friend inline auto operator|(_range&& lhs, __append_to_helper rhs) mr_noexcept {
		return append_to(std::forward<_range>(lhs), rhs._dst);
	}
};

template <typename Container>
MR_NODISCARD inline auto append_to(Container& dst) {
	return __append_to_helper{ dst };
}

template <range _range, typename Container>
MR_NODISCARD inline auto emplace_back_to(_range&& r, Container& dst) {
	struct emplace_back_to_wrapper {
		_range _r;
		Container& dst;
		inline auto operator()() mr_noexcept {
			auto fn = [&](auto&&... args) { dst.emplace_back(std::forward<decltype(args)>(args)...); };
			for (auto&& i : _r) std::apply(fn, i);
		}
	};
	return emplace_back_to_wrapper{ std::forward<_range>(r), dst };
}

template <typename Container>
struct __emplace_back_to_helper { 
	Container& _dst;
	template <range _range>
	MR_NODISCARD friend inline auto operator|(_range&& lhs, __emplace_back_to_helper rhs) mr_noexcept {
		return emplace_back_to(std::forward<_range>(lhs), rhs._dst);
	}
};

template <typename Container>
MR_NODISCARD inline auto emplace_back_to(Container& dst) {
	return __emplace_back_to_helper{ dst };
}

constexpr inline struct {
} endp;

inline auto operator|(auto&& lhs, const decltype(endp)&) mr_noexcept {
	return lhs();
}
MR_NAMESPACE_END