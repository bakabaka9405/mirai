#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/range.hpp>
MR_NAMESPACE_BEGIN
template <typename iter_t>
constexpr auto from(iter_t&& it) {
	struct from_wrapper {
		iter_t it;
		inline constexpr auto begin() const mr_noexcept { return it; }
		inline constexpr auto end() const mr_noexcept { return std::unreachable_sentinel; }
	};
	return from_wrapper{ std::forward<iter_t>(it) };
}

template <range range>
inline constexpr auto in_array(range&& arr, ull l, ull r) {
	struct in_array_wrapper {
		range arr;
		ull l, r;
		inline constexpr auto begin() const mr_noexcept { return std::next(mr_begin(arr), l); }
		inline constexpr auto end() const mr_noexcept { return std::next(mr_begin(arr), r + 1); }
	};
	return in_array_wrapper{ std::forward<range>(arr), l, r };
}
class __cycle_helper { 
private:
	template <range _range>
	inline constexpr auto invoke(_range&& r) mr_noexcept {
		struct cycle_wrapper {
			_range _r;
			struct cycled_iterator {
				using iter_t = decltype(mr_begin(_r));
				iter_t _it;
				_range& rg;
				inline bool operator!=(const std::default_sentinel_t&) const mr_noexcept {
					return true;
				}
				inline decltype(auto) operator*() const mr_noexcept {
					return *_it;
				}
				inline cycled_iterator& operator++() mr_noexcept {
					if (++_it == mr_end(rg)) _it = mr_begin(rg);
					return *this;
				}
				inline auto operator++(int) mr_noexcept->cycled_iterator {
					cycled_iterator res = *this;
					this->operator++();
					return res;
				}
			};
			inline auto begin() const mr_noexcept { return cycled_iterator{ mr_begin(_r), _r }; }
			inline auto end() const mr_noexcept { return std::unreachable_sentinel; }
		};
		return cycle_wrapper{ std::forward<_range>(r) };
	}

public:
	template <range _range>
	MR_NODISCARD friend inline auto operator|(_range&& lhs, __cycle_helper rhs) mr_noexcept {
		return rhs.invoke(std::forward<_range>(lhs));
	}

	template <range _range>
	MR_NODISCARD auto operator()(_range&& r) const mr_noexcept {
		return invoke(std::forward<_range>(r));
	}
} constexpr cycle;

template <range _range>
inline constexpr auto take(_range&& r, size_t n) mr_noexcept {
	struct take_wrapper {
		_range _r;
		size_t _n;
		struct iterator {
			using iter_t = decltype(mr_begin(_r));

			iter_t _it;
			size_t _count;

			inline bool operator!=(const std::default_sentinel_t&) const mr_noexcept {
				return _count;
			}
			inline decltype(auto) operator++() mr_noexcept {
				++_it;
				--_count;
				return *this;
			}
			inline auto operator++(int) mr_noexcept->iterator {
				return iterator{ _it++, _count-- };
			}
			inline decltype(auto) operator*() const mr_noexcept {
				return *_it;
			}
		};

		inline auto begin() const mr_noexcept {
			return iterator{ mr_begin(_r), _n };
		}

		inline constexpr auto end() const mr_noexcept {
			return std::default_sentinel;
		}
	};
	return take_wrapper{ std::forward<_range>(r), n };
}

struct __take_helper { 
	size_t n;
	template <range _range>
	MR_NODISCARD friend inline auto operator|(_range&& lhs, __take_helper self) mr_noexcept {
		return take(std::forward<_range>(lhs), self.n);
	}
};

MR_NODISCARD inline constexpr auto take(size_t n) {
	return __take_helper{ n };
}

template <range _range>
MR_NODISCARD inline constexpr auto in_column(_range&& r, ull col) {
	struct in_column_wrapper {
		_range _r;
		const ull _col;
		struct iterator {
			using iter_t = decltype(mr_begin(_r));
			_range& _rg;
			iter_t _it;
			const ull _col;
			inline bool operator!=(const std::default_sentinel_t&) const mr_noexcept {
				return _it != mr_end(_rg);
			}
			inline decltype(auto) operator++() mr_noexcept {
				++_it;
			}
			inline auto operator++(int) mr_noexcept->iterator {
				return iterator{ _rg, _it++, _col };
			}
			inline auto operator*() const mr_noexcept {
				return (*_it)[_col];
			}
		};
		inline auto begin() const mr_noexcept {
			return iterator{ _r, mr_begin(_r), _col };
		}
		inline constexpr auto end() const mr_noexcept {
			return std::default_sentinel;
		}
	};
	return in_column_wrapper{ std::forward<_range>(r), col };
}

struct __in_column_helper { 
	const ull col;
	template <range _range>
	MR_NODISCARD friend inline auto operator|(_range&& lhs, __in_column_helper self) {
		return in_column(std::forward<_range>(lhs), self.col);
	}
};

MR_NODISCARD inline constexpr auto in_column(ull col) {
	return __in_column_helper{ col };
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
	MR_NODISCARD friend inline auto operator|(auto&& lhs, __skip_helper self) mr_noexcept {
		return skip(std::forward<decltype(lhs)>(lhs), self.n);
	}
};

inline constexpr auto skip(size_t n) {
	return __skip_helper{ n };
}

void repeat(size_t n, auto&& func) mr_noexcept {
	while (n--) func();
}

MR_NAMESPACE_END
