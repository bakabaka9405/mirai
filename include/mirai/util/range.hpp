#pragma once
#include <mirai/pch.hpp>
namespace mirai {
	inline auto zip(auto&& arr1, auto&& arr2) mr_noexcept {
		using arr1_t = decltype(arr1);
		using arr2_t = decltype(arr2);
		using iter1_t = decltype(std::begin(arr1));
		using iter2_t = decltype(std::begin(arr2));

		struct zip_wrapper {
			struct iterator {
				iter1_t _it1;
				iter2_t _it2;
				inline bool operator!=(const iterator& rt) const mr_noexcept {
					return _it1 != rt._it1 && _it2 != rt._it2;
				}
				inline void operator++() mr_noexcept {
					++_it1;
					++_it2;
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

			arr1_t _arr1;
			arr2_t _arr2;
		};
		return zip_wrapper{ arr1, arr2 };
	}

	inline auto enumerate(auto&& arr) mr_noexcept {
		using arr_t = decltype(arr);
		using iter_t = decltype(std::begin(arr));

		struct enum_wrapper {
			struct iterator {
				ll _index;
				iter_t _it;
				inline bool operator!=(const iterator& rt) const mr_noexcept {
					return _it != rt._it;
				}
				inline void operator++() mr_noexcept {
					++_index;
					++_it;
				}
				inline auto operator*() const mr_noexcept {
					// return std::pair<typename std::iterator_traits<iter_t>::value_type, const size_t>{ *_it, _index };
					return pair<ll, decltype(*_it)>{ _index, *_it };
				}
			};

			auto begin() mr_noexcept {
				return iterator{ 0ll, std::begin(_data) };
			}

			auto end() mr_noexcept {
				return iterator{ static_cast<ll>(std::size(_data)), std::end(_data) };
			}
			arr_t _data;
		};
		return enum_wrapper{ arr };
	}
} // namespace mirai