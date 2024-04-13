#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
template <typename T>
	requires requires(T&& v) { v.begin(); } || requires(T&& v) { std::begin(v); }
inline auto mr_begin(T&& v) mr_noexcept {
	if constexpr (requires { v.begin(); })
		return v.begin();
	else
		return std::begin(v);
}
template <typename T>
	requires requires(T&& v) { v.end(); } || requires(T&& v) { std::end(v); }
inline auto mr_end(T&& v) mr_noexcept {
	if constexpr (requires { v.end(); })
		return v.end();
	else
		return std::end(v);
}

template <typename T>
concept range = requires(T&& t) {
	mr_begin(t);
	mr_end(t);
};

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
	inline void operator++(int) mr_noexcept {
		++first, ++second;
	}
	inline decltype(auto) operator*() const mr_noexcept {
		return value_type{ *first, *second };
	}
};

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

constexpr auto enumerate(range auto&& arr) mr_noexcept {
	return zip(views::iota(0ll), std::forward<decltype(arr)>(arr));
}

MR_NAMESPACE_END