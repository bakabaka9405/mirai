#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/range.hpp>
namespace mirai {
	template <range _range>
	inline auto take(_range&& r, size_t n) mr_noexcept {
		struct take_wrapper {
			_range _r;
			size_t _n;
			struct iterator {
				using iter_t = decltype(mr_begin(_r));
				using value_type = std::decay<decltype(*(declval<iter_t>()))>;
				using difference_type = ll;
				using pointer = value_type*;
				using reference = value_type&;
				using iterator_category = std::forward_iterator_tag;

				iter_t _it;
				size_t _index;

				inline bool operator!=(const iterator& rt) const mr_noexcept {
					return _index != rt._index;
				}
				inline bool operator==(const iterator& rt) const mr_noexcept {
					return !this->operator==(rt);
				}
				inline decltype(auto) operator++() mr_noexcept {
					++_it;
					++_index;
					return *this;
				}
				inline auto operator++(int) mr_noexcept {
					return iterator{ _it++, _index++ };
				}
				inline auto operator*() const mr_noexcept {
					return *_it;
				}
			};

			inline auto begin() const mr_noexcept {
				return iterator{ mr_begin(_r), 0 };
			}

			inline auto end() const mr_noexcept {
				return default_pair_sentinel{ mr_end(_r), _n };
			}
		};
		return take_wrapper{ std::forward<_range>(r), n };
	}

	struct __take_helper {
		size_t n;
		friend inline auto operator|(auto&& lhs, const __take_helper& self) mr_noexcept {
			return take(std::forward<decltype(lhs)>(lhs), self.n);
		}
	};

	inline auto take(size_t n) {
		return __take_helper{ n };
	}

	template <range _range, typename Func>
	inline auto transform(_range&& r, Func func) {
		struct transform_wrapper {
			_range _r;
			Func _func;
			struct sentinel {
				decltype(mr_end(_r)) _it;
			};
			struct iterator {
				using iter_t = decltype(mr_begin(_r));
				iter_t _it;
				Func _func;
				using value_type = decltype(_func(*_it));
				inline bool operator!=(const sentinel& rt) const mr_noexcept {
					return _it != rt._it;
				}
				inline bool operator==(const sentinel& rt) const mr_noexcept {
					return !this->operator==(rt);
				}
				inline decltype(auto) operator++() mr_noexcept {
					++_it;
					return *this;
				}
				inline auto operator++(int) mr_noexcept {
					return iterator{ _it++ };
				}
				inline auto operator*() const mr_noexcept {
					return _func(*_it);
				}
			};
			inline auto begin() const mr_noexcept { return iterator{ mr_begin(_r), _func }; }
			inline auto end() const mr_noexcept { return sentinel{ mr_end(_r) }; }
		};
		return transform_wrapper{ std::forward<_range>(r), func };
	};

	template <typename Func>
	struct __transform_helper {
		Func func;
		friend inline auto operator|(auto&& lhs, const __transform_helper& self) mr_noexcept {
			return transform(std::forward<decltype(lhs)>(lhs), self.func);
		}
		friend inline auto operator|(auto&& lhs, __transform_helper&& self) mr_noexcept {
			return transform(std::forward<decltype(lhs)>(lhs), std::forward<Func>(self.func));
		}
	};

	template <typename Func>
	inline auto transform(Func func) {
		return __transform_helper{ std::forward<Func>(func) };
	}

	template <range _range>
	inline auto skip(_range&& r, size_t n) {
		struct skip_wrapper {
			_range r;
			size_t _n;
			inline auto begin() const mr_noexcept {
				auto p = mr_begin(r);
				auto e = mr_end(r);
				for (size_t i = _n; i && p != e; --i) ++p;
				return p;
			};
			inline auto end() const mr_noexcept {
				return mr_end(r);
			}
		};
		return skip_wrapper{ std::forward<_range>(r), n };
	}

	struct __skip_helper {
		size_t n;
		friend inline auto operator|(auto&& lhs, const __skip_helper& self) mr_noexcept {
			return skip(std::forward<decltype(lhs)>(lhs), self.n);
		}
	};

	inline auto skip(size_t n) {
		return __skip_helper{ n };
	}
} // namespace mirai
