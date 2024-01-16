#pragma once
#include <mirai/pch.hpp>
namespace mirai {
	template <typename _range1, typename _range2>
	inline auto zip(_range1&& arr1, _range2&& arr2) mr_noexcept {
		using iter1_t = decltype(std::begin(arr1));
		using iter2_t = decltype(std::begin(arr2));

		struct zip_wrapper {
			struct iterator {
				iter1_t _it1;
				iter2_t _it2;
				using value_type = pair<decltype(*_it1), decltype(*_it2)>;
				using difference_type = ll;
				using pointer = value_type*;
				using reference = value_type&;
				using iterator_category = std::forward_iterator_tag;
				inline bool operator!=(const iterator& rt) const mr_noexcept {
					return _it1 != rt._it1 && _it2 != rt._it2;
				}
				inline bool operator==(const iterator& rt) const mr_noexcept {
					return !this->operator==(rt);
				}
				inline decltype(auto) operator++() mr_noexcept {
					++_it1, ++_it2;
					return *this;
				}
				inline auto operator++(int) mr_noexcept {
					return iterator{ _it1++, _it2++ };
				}
				inline auto operator*() const mr_noexcept {
					return pair<decltype(*_it1), decltype(*_it2)>{ *_it1, *_it2 };
				}
			};

			auto begin() mr_noexcept {
				return iterator{ std::begin(_arr1), std::begin(_arr2) };
			}

			auto end() mr_noexcept {
				return iterator{ std::end(_arr1), std::end(_arr2) };
			}

			_range1 _arr1;
			_range2 _arr2;
		};
		return zip_wrapper{ std::forward<_range1>(arr1), std::forward<_range2>(arr2) };
	}

	template <typename _range>
	inline auto enumerate(_range&& arr) mr_noexcept {
		using iter_t = decltype(std::begin(arr));

		struct enum_wrapper {
			struct iterator {
				ll _index;
				iter_t _it;
				inline bool operator!=(const iterator& rt) const mr_noexcept {
					return _it != rt._it;
				}
				inline bool operator==(const iterator& rt) const mr_noexcept {
					return !this->operator!=(rt);
				}
				inline decltype(auto) operator++() mr_noexcept {
					++_index, ++_it;
					return *this;
				}
				inline auto operator++(int) mr_noexcept {
					return iterator{ _index++, _it++ };
				}

				inline auto operator*() const mr_noexcept {
					// return std::pair<typename std::iterator_traits<iter_t>::value_type, const size_t>{ *_it, _index };
					return pair<ll, decltype(*_it)>{ _index, *_it };
				}
				using value_type = pair<ll, decltype(*_it)>;
				using difference_type = ll;
				using pointer = value_type*;
				using reference = value_type&;
				using iterator_category = std::forward_iterator_tag;
			};

			auto begin() mr_noexcept {
				return iterator{ 0ll, std::begin(_data) };
			}

			auto end() mr_noexcept {
				return iterator{ static_cast<ll>(std::size(_data)), std::end(_data) };
			}

			_range _data;
		};

		return enum_wrapper{ std::forward<_range>(arr) };
	}
} // namespace mirai