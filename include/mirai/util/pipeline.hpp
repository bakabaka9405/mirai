#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/range.hpp>
MR_NAMESPACE_BEGIN
template <typename iter_t>
MR_NODISCARD inline constexpr auto from(iter_t&& it) {
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
		inline constexpr auto begin() const mr_noexcept { return mr_begin(arr) + l; }
		inline constexpr auto end() const mr_noexcept { return mr_begin(arr) + r + 1; }
	};
	return in_array_wrapper{ std::forward<range>(arr), l, r };
}

template <range _range>
inline constexpr auto take(_range&& r, size_t n) mr_noexcept {
	struct take_wrapper {
		_range _r;
		size_t _n;
		struct iterator {
			using iter_t = decltype(mr_begin(_r));

			iter_t _it;
			size_t _count;

			inline bool operator!=(const std::default_sentinel_t& rt) const mr_noexcept {
				return _count;
			}
			inline decltype(auto) operator++() mr_noexcept {
				++_it;
				--_count;
				return *this;
			}
			inline auto operator++(int) mr_noexcept {
				return iterator{ _it++, _count-- };
			}
			inline auto operator*() const mr_noexcept {
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
	friend inline auto operator|(auto&& lhs, __take_helper self) mr_noexcept {
		return take(std::forward<std::decay_t<decltype(lhs)>>(lhs), self.n);
	}
};

MR_NODISCARD inline constexpr auto take(size_t n) {
	return __take_helper{ n };
}

template <range _range, typename Func>
inline constexpr auto transform(_range&& r, Func&& func) {
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
				return !this->operator!=(rt);
			}
			inline decltype(auto) operator++() mr_noexcept {
				++_it;
				return *this;
			}
			inline auto operator++(int) mr_noexcept {
				iterator res = *this;
				this->operator++();
				return res;
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
	template <range _range>
	friend inline auto operator|(_range&& lhs, __transform_helper self) mr_noexcept {
		return transform(std::forward<_range>(lhs), std::move(self.func));
	}
};

template <typename Func>
inline constexpr auto transform(Func&& func) {
	return __transform_helper{ std::forward<Func>(func) };
}

template <range _range, typename Func>
inline auto filter(_range&& r, Func&& func) {
	struct filter_wrapper {
		_range _r;
		Func _func;
		struct sentinel {
			decltype(mr_end(_r)) _it;
		};
		struct iterator {
			using iter_t = decltype(mr_begin(_r));
			iter_t _it;
			Func _func;
			sentinel _end;
			using value_type = decltype(*_it);
			inline bool operator!=(const sentinel& rt) const mr_noexcept {
				return _it != rt._it;
			}
			inline bool operator==(const sentinel& rt) const mr_noexcept {
				return !this->operator!=(rt);
			}
			inline decltype(auto) operator++() mr_noexcept {
				do {
					++_it;
				} while (_it != _end._it && !_func(*_it));
				return *this;
			}
			inline auto operator++(int) mr_noexcept {
				iterator it = *this;
				this->operator++();
				return it;
			}
			inline auto operator*() const mr_noexcept->value_type {
				return *_it;
			}
		};
		inline auto begin() const mr_noexcept {
			auto it = mr_begin(_r);
			auto end = mr_end(_r);
			while (it != end && !_func(*it)) ++it;
			return iterator{ it, _func, sentinel{ end } };
		}
		inline auto end() const mr_noexcept { return sentinel{ mr_end(_r) }; }
	};
	return filter_wrapper{ std::forward<_range>(r), std::forward<Func>(func) };
};
template <typename Func>
struct __filter_helper {
	Func func;

	template <range _range>
	friend inline auto operator|(_range&& lhs, __filter_helper self) mr_noexcept {
		return filter(std::forward<_range>(lhs), std::move(self.func));
	}
};

template <typename Func>
inline auto filter(Func&& func) {
	return __filter_helper{ std::forward<Func>(func) };
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
	friend inline auto operator|(auto&& lhs, __skip_helper self) mr_noexcept {
		return skip(std::forward<decltype(lhs)>(lhs), self.n);
	}
};

inline constexpr auto skip(size_t n) {
	return __skip_helper{ n };
}

template <typename T, range _range, typename Func>
MR_NODISCARD inline auto extreme_value(_range&& r, Func cmp) {
	struct extreme_value_wrapper {
		_range _r;
		Func _cmp;
		inline auto operator()() const mr_noexcept->std::optional<T> {
			std::optional<T> res;
			for (auto&& i : _r) {
				if (!res.has_value() || _cmp(i, *res)) res = i;
			}
			return res;
		}
	};
	return extreme_value_wrapper{ std::forward<_range>(r), cmp };
}

template <typename T, typename Func>
struct __extreme_value_helper {
	Func _cmp;
	template <range _range>
	friend inline auto operator|(_range&& lhs, __extreme_value_helper rhs) mr_noexcept {
		return extreme_value<T>(std::forward<_range>(lhs), std::move(rhs._cmp));
	}
};

template <typename T, typename Func>
inline auto extreme_value(Func cmp) {
	return __extreme_value_helper<T, Func>{ std::forward<Func>(cmp) };
}

template <typename T>
struct __maximum {
	constexpr static auto _cmp = std::greater<T>();
	inline decltype(auto) operator()(range auto&& r) const mr_noexcept {
		return extreme_value<T>(std::forward<decltype(r)>(r), _cmp);
	}
	friend inline decltype(auto) operator|(range auto&& r, const __maximum&) mr_noexcept {
		return extreme_value<T>(std::forward<decltype(r)>(r), _cmp);
	}
};

template <typename T>
struct __minimum {
	constexpr static auto _cmp = std::less<T>();
	inline decltype(auto) operator()(range auto&& r) const mr_noexcept {
		return extreme_value<T>(std::forward<decltype(r)>(r), _cmp);
	}
	friend inline decltype(auto) operator|(range auto&& r, const __minimum&) mr_noexcept {
		return extreme_value<T>(std::forward<decltype(r)>(r), _cmp);
	}
};

template <typename T>
inline constexpr __maximum<T> maximum;

template <typename T>
inline constexpr __minimum<T> minimum;

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
	friend inline auto operator|(_range&& lhs, __addup_to_helper rhs) mr_noexcept {
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
struct __save_to_helper { // NOLINT(bugprone-reserved-identifier)
	T _dst;
	template <range _range>
	friend inline auto operator|(_range&& lhs, __save_to_helper rhs) mr_noexcept {
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
			for (auto&& i : _r) *it = std::forward<std::decay_t<decltype(i)>>(i);
		}
	};
	return append_to_wrapper{ std::forward<_range>(r), dst };
}

template <typename Container>
struct __append_to_helper { // NOLINT(bugprone-reserved-identifier)
	Container& _dst;
	template <range _range>
	friend inline auto operator|(_range&& lhs, __append_to_helper rhs) mr_noexcept {
		return append_to(std::forward<_range>(lhs), rhs._dst);
	}
};

template <typename Container>
MR_NODISCARD inline auto append_to(Container& dst) {
	return __append_to_helper{ dst };
}

template <range range, typename Container>
inline constexpr auto maps_to(range&& r, Container&& table) {
	struct maps_to_wrapper {
		using iter_t = decltype(mr_begin(r));
		using sen_t = decltype(mr_end(r));
		range _r;
		Container table;
		struct sentinel {
			sen_t sen;
		};
		struct iterator {
			iter_t it;
			Container table;
			inline bool operator!=(const sentinel& rt) const { return it != rt.sen; }
			inline decltype(auto) operator*() const { return table[*it]; }
			inline iterator& operator++() {
				++it;
				return *this;
			}
		};
		inline constexpr auto begin() { return iterator{ mr_begin(_r), std::forward<Container>(table) }; }
		inline constexpr auto end() const { return sentinel{ mr_end(_r) }; }
	};
	return maps_to_wrapper{ std::forward<range>(r), std::forward<Container>(table) };
}

template <typename Container>
struct __maps_to_helper { // NOLINT(bugprone-reserved-identifier)
	Container table;
	template <range range>
	friend inline decltype(auto) operator|(range&& r, __maps_to_helper self) {
		return maps_to(std::forward<range>(r), std::move(self.table));
	}
};

template <typename Container>
inline constexpr auto maps_to(Container&& table) {
	return __maps_to_helper{ std::forward<Container>(table) };
}

constexpr inline struct {
} endp;

inline auto operator|(auto&& lhs, const decltype(endp)&) mr_noexcept {
	return lhs();
}

inline constexpr auto as_abs = [](auto x) { return std::abs(x); };

inline constexpr auto as_square = [](auto x) { return x * x; };

inline constexpr auto is_positive = [](auto x) { return x > 0; };

inline constexpr auto not_negative = [](auto x) { return x >= 0; };

inline constexpr auto to_pair = [](auto&& x) { return pair{ get<0>(x), get<1>(x) }; };

MR_NAMESPACE_END
