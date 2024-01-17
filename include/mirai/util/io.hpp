#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/string.hpp>
#include <utility>
namespace mirai {
	template <typename T>
	inline void read_array(auto&& it, size_t read_n) mr_noexcept {
		copy_n(istream_iterator<T>(cin), read_n, it);
	}

	inline void put_array(auto&& begin, auto&& end, const char* sep = " ") mr_noexcept {
		auto pt = begin;
		cout << *pt++;
		while (pt != end) {
			cout << sep << *pt++;
		}
	}

	inline void put_array(auto&& r, const char* sep = " ") mr_noexcept {
		put_array(std::begin(r), std::end(r), sep);
	}

	inline std::istream& operator>>(std::istream& in, i128& v) {
		string s;
		in >> s;
		v = stoi128(s);
		return in;
	}

	inline std::ostream& operator<<(std::ostream& out, i128 v) {
		return out << to_string(v);
	}

	inline ll readi() {
		ll x = 0;
		bool f = 0;
		char c = getchar();
		while (c < '0' || c > '9') {
			if (c == '-') f = 1;
			c = getchar();
		}
		while (c >= '0' && c <= '9') {
			x = (x << 1) + (x << 3) + (c ^ 48);
			c = getchar();
		}
		return f ? -x : x;
	}

	inline void puti(integral auto x) {
		if (x < 0) putchar('-'), x = -x;
		if (x > 9) puti(x / 10);
		putchar(x % 10 + '0');
	}

	template <typename... Args>
	inline void input(Args&&... args) {
		((cin >> args), ...);
	}

	template <typename... Args>
	inline void output(Args&&... args) {
		((cout << args), ...);
	}

	template <typename... Args, size_t... I>
	inline auto read_tuple(std::index_sequence<I...>) {
		tuple<Args...> res;
		((cin >> std::get<I>(res)), ...);
		return res;
	}

	template <typename... Args>
	inline auto read_tuple() {
		return []<size_t... I>(std::index_sequence<I...>) {
			std::tuple<Args...> res;
			((cin >> std::get<I>(res)), ...);
			return res;
		}(std::make_index_sequence<sizeof...(Args)>());
	}
} // namespace mirai