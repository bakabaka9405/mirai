#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/concepts.hpp>
MR_NAMESPACE_BEGIN
namespace detail {
	template <typename... iter_t>
	struct default_tuple_forward_iterator;
	template <typename... iter_t>
	struct default_tuple_sentinel;
} // namespace detail
MR_NAMESPACE_END
namespace std {
	template <typename... iter_t>
	struct tuple_size<::mirai::detail::default_tuple_forward_iterator<iter_t...>> : integral_constant<size_t, sizeof...(iter_t)> {}; // NOLINT(cert-dcl58-cpp)
	template <typename... iter_t>
	struct tuple_size<::mirai::detail::default_tuple_sentinel<iter_t...>> : integral_constant<size_t, sizeof...(iter_t)> {}; // NOLINT(cert-dcl58-cpp)
} // namespace std
MR_NAMESPACE_BEGIN
namespace detail {
	template <typename T, typename Y, size_t... I>
	constexpr bool tuple_all_elements_not_eq_impl(const T& t, const Y& y, std::index_sequence<I...>) mr_noexcept {
		return ((std::get<I>(t) != std::get<I>(y)) && ...);
	}

	template <typename T, typename Y>
	constexpr bool tuple_all_elements_not_eq(const T& t, const Y& y) mr_noexcept {
		return tuple_all_elements_not_eq_impl(t, y, std::make_index_sequence<std::tuple_size_v<std::remove_cvref_t<T>>>());
	}

	template <typename T, size_t... I>
	inline void tuple_all_elements_increase_impl(T& t, std::index_sequence<I...>) mr_noexcept {
		(++std::get<I>(t), ...);
	}

	template <typename T>
	inline void tuple_all_elements_increase(T& t) mr_noexcept {
		tuple_all_elements_increase_impl(t, std::make_index_sequence<std::tuple_size_v<T>>());
	}

	template <typename T, size_t... I>
	constexpr decltype(auto) tuple_dereference_impl(const T& t, std::index_sequence<I...>) mr_noexcept {
		return std::tuple<decltype(*std::get<I>(t))...>(*std::get<I>(t)...);
	}

	template <typename T>
	constexpr decltype(auto) tuple_dereference(const T& t) mr_noexcept {
		return tuple_dereference_impl(t, std::make_index_sequence<std::tuple_size_v<std::remove_cvref_t<T>>>());
	}

	template <typename... iter_t>
	struct default_tuple_forward_iterator : public tuple<iter_t...> {
		explicit constexpr default_tuple_forward_iterator(iter_t&&... iters)
			: tuple<iter_t...>{ std::forward<iter_t>(iters)... } {}

		inline decltype(auto) operator++() mr_noexcept {
			tuple_all_elements_increase(*this);
			return *this;
		}
		inline auto operator++(int) mr_noexcept->default_tuple_forward_iterator {
			auto res = *this;
			tuple_all_elements_increase(*this);
			return res;
		}
		inline decltype(auto) operator*() const mr_noexcept {
			return tuple_dereference(*this);
		}
	};

	template <typename... iter_t>
	default_tuple_forward_iterator(iter_t&&...) -> default_tuple_forward_iterator<iter_t...>;

	template <typename... iter_t>
	struct default_tuple_sentinel : public tuple<iter_t...> {
		explicit constexpr default_tuple_sentinel(iter_t&&... iters)
			: tuple<iter_t...>{ std::forward<iter_t>(iters)... } {}

		template <typename Iterator>
			requires(std::tuple_size_v<Iterator> == sizeof...(iter_t))
		friend bool operator!=(const Iterator& lhs, const default_tuple_sentinel& self) mr_noexcept {
			return tuple_all_elements_not_eq(lhs, self);
		}
		template <typename Iterator>
			requires(std::tuple_size_v<Iterator> == sizeof...(iter_t))
		friend bool operator==(const Iterator& lhs, const default_tuple_sentinel& self) mr_noexcept {
			return !(lhs != self);
		}
	};
	template <typename... iter_t>
	default_tuple_sentinel(iter_t&&...) -> default_tuple_sentinel<iter_t...>;
} // namespace detail

template <range... _ranges>
constexpr auto zip(_ranges&&... arr) mr_noexcept {
	static_assert(sizeof...(_ranges) > 1, "zip must have at least 2 ranges");
	struct zip_wrapper {
		auto begin() const mr_noexcept {
			return std::apply([](auto&&... rg) {
				return detail::default_tuple_forward_iterator{ mr_begin(rg)... };
			},
							  _arrs);
		}
		auto end() const mr_noexcept {
			return std::apply([](auto&&... rg) {
				return detail::default_tuple_sentinel{ mr_end(rg)... };
			},
							  _arrs);
		}
		std::tuple<_ranges...> _arrs;
	};
	return zip_wrapper{ std::forward_as_tuple(arr...) };
};

template <range _range>
constexpr auto enumerate(_range&& arr, size_t start = 0) mr_noexcept {
	return zip(views::iota(start), std::forward<_range>(arr));
}

template <range _range>
using range_element_t = decltype(*mr_begin(std::declval<_range>()));

MR_NAMESPACE_END