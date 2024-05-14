#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/concepts.hpp>
MR_NAMESPACE_BEGIN

template <typename sentinel1_t, typename sentinel2_t>
struct default_pair_sentinel {
	sentinel1_t _st1;
	sentinel2_t _st2;
	friend bool operator!=(auto&& lt, const default_pair_sentinel& rt) mr_noexcept {
		auto&& [i, j] = lt;
		return i != rt._st1 && j != rt._st2;
	}
	friend bool operator==(auto&& lt, const default_pair_sentinel& rt) mr_noexcept { return !(lt != rt); }
};

template <typename iter1_t, typename iter2_t>
struct default_pair_forward_iterator {
	iter1_t first;
	iter2_t second;
	using value_type = pair<decltype(*declval<iter1_t>()), decltype(*declval<iter2_t>())>;
	using difference_type = ll;
	using pointer = value_type*;
	using reference = value_type&;
	using iterator_category = std::forward_iterator_tag;
	inline bool operator!=(const default_pair_forward_iterator& rt) const mr_noexcept {
		return first != rt.first && second != rt.second;
	}
	inline bool operator==(const default_pair_forward_iterator& rt) const mr_noexcept {
		return !this->operator==(rt);
	}
	inline decltype(auto) operator++() mr_noexcept {
		++first, ++second;
		return *this;
	}
	inline auto operator++(int) mr_noexcept->default_pair_forward_iterator {
		auto res = *this;
		this->operator++();
		return res;
	}
	inline decltype(auto) operator*() const mr_noexcept {
		return value_type{ *first, *second };
	}
};

namespace detail {
	template <typename T, typename Y, size_t... I>
	bool tuple_all_elements_not_eq_impl(T&& t, Y&& y, std::index_sequence<I...>) {
		return (!std::equal_to<>{}(std::get<I>(t), std::get<I>(y)) && ...);
	}

	template <typename T, typename Y, size_t N>
	bool tuple_all_elements_not_eq(T&& t, Y&& y) {
		return tuple_all_elements_not_eq_impl(std::forward<T>(t), std::forward<Y>(y), std::make_index_sequence<N>());
	}

	template <typename T, size_t... I>
	void tuple_all_elements_increase_impl(T& t, std::index_sequence<I...>) {
		(std::get<I>(t)++, ...);
	}

	template <typename T, size_t N>
	bool tuple_all_elements_increase(T& t) {
		return tuple_all_elements_increase_impl(t, std::make_index_sequence<N>());
	}

	template <typename... sentinel_t>
	struct default_tuple_sentinel {
		tuple<sentinel_t...> sen;
		template <typename Iterator>
			requires(std::tuple_size_v<Iterator> == sizeof...(sentinel_t))
		friend bool operator!=(Iterator&& lhs, const default_tuple_sentinel& self) mr_noexcept {
			return tuple_all_elements_not_eq_impl(std::forward<Iterator>(lhs), self, sizeof...(sentinel_t));
		}
		template <typename Iterator>
			requires(std::tuple_size_v<Iterator> == sizeof...(sentinel_t))
		friend bool operator==(Iterator&& lhs, const default_tuple_sentinel& self) mr_noexcept {
			return !(lhs != self);
		}
		inline decltype(auto) operator++() mr_noexcept {
			tuple_all_elements_increase(sen, sizeof...(sentinel_t));
			return *this;
		}
		inline void operator++(int) mr_noexcept {
			tuple_all_elements_increase(sen, sizeof...(sentinel_t));
		}
	};
} // namespace detail

template <range _range1, range _range2>
constexpr auto zip(_range1&& arr1, _range2&& arr2) mr_noexcept {
	struct zip_wrapper {
		auto begin() const mr_noexcept { return default_pair_forward_iterator{ mr_begin(_arr1), mr_begin(_arr2) }; }
		auto end() const mr_noexcept { return default_pair_sentinel{ mr_end(_arr1), mr_end(_arr2) }; }
		_range1 _arr1;
		_range2 _arr2;
	};
	return zip_wrapper{ std::forward<_range1>(arr1), std::forward<_range2>(arr2) };
}

template <range _range>
constexpr auto enumerate(_range&& arr) mr_noexcept {
	return zip(views::iota(0ll), std::forward<_range>(arr));
}

MR_NAMESPACE_END