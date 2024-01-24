#pragma once
#include <mirai/pch.hpp>
namespace mirai {
	inline auto mr_begin(auto&& v) mr_noexcept {
		if constexpr (requires { v.begin(); })
			return v.begin();
		else if constexpr (requires { std::begin(v); })
			return std::begin(v);
		else
			static_assert(false, "parameter doesn't meet the constraints of mr_begin.");
	}
	inline auto mr_end(auto&& v) mr_noexcept {
		if constexpr (requires { v.end(); })
			return v.end();
		else if constexpr (requires { std::end(v); })
			return std::end(v);
		else
			static_assert(false, "parameter doesn't meet the constraints of mr_begin.");
	}

	template <typename T>
	concept range = requires(T&& t) {
		mr_begin(t);
		mr_end(t);
	};

	template <typename sentinel1_t, typename sentinel2_t>
	struct pair_sentinel {
		sentinel1_t _st1;
		sentinel2_t _st2;
		friend bool operator!=(auto&& lt, const pair_sentinel& rt) mr_noexcept {
			auto&& [i, j] = lt;
			return i != rt._st1 && j != rt._st2;
		}
		friend bool operator==(auto&& lt, const pair_sentinel& rt) mr_noexcept { return !(lt != rt); }
	};

	template <typename iter1_t, typename iter2_t>
	struct pair_forward_iterator {
		iter1_t first;
		iter2_t second;
		using value_type = pair<decltype(*declval<iter1_t>()), decltype(*declval<iter2_t>())>;
		using difference_type = ll;
		using pointer = value_type*;
		using reference = value_type&;
		using iterator_category = std::forward_iterator_tag;
		inline bool operator!=(const pair_forward_iterator& rt) const mr_noexcept {
			return first != rt._it1 && second != rt._it2;
		}
		inline bool operator==(const pair_forward_iterator& rt) const mr_noexcept {
			return !this->operator==(rt);
		}
		inline decltype(auto) operator++() mr_noexcept {
			++first, ++second;
			return *this;
		}
		inline auto operator++(int) mr_noexcept {
			return pair_forward_iterator{ first++, second++ };
		}
		inline auto operator*() const mr_noexcept {
			return value_type{ *first, *second };
		}
	};

	template <range _range1, range _range2>
	inline auto zip(_range1&& arr1, _range2&& arr2) mr_noexcept {
		struct zip_wrapper {
			auto begin() mr_noexcept {
				return pair_forward_iterator{ std::begin(_arr1), std::begin(_arr2) };
			}

			auto end() mr_noexcept {
				return pair_sentinel{ std::end(_arr1), std::end(_arr2) };
			}

			_range1 _arr1;
			_range2 _arr2;
		};
		return zip_wrapper{ std::forward<_range1>(arr1), std::forward<_range2>(arr2) };
	}

	template <typename _range>
	inline auto enumerate(_range&& arr) mr_noexcept {
		return zip(views::iota(0), std::forward<_range>(arr));
	}
} // namespace mirai