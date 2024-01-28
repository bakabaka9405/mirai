#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/string.hpp>
namespace mirai {
#if MR_HAVE_INT128
	inline std::istream& operator>>(std::istream& in, i128& v) {
		string s;
		in >> s;
		v = stoi128(s);
		return in;
	}

	inline std::ostream& operator<<(std::ostream& out, i128 v) {
		return out << to_string(v);
	}
#endif
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

	template <typename T, typename Y>
	inline std::istream& operator>>(std::istream& in, pair<T, Y>& p) {
		return in >> p.first >> p.second;
	}

	template <typename T, typename Y>
	inline std::ostream& operator<<(std::ostream& out, const pair<T, Y>& p) {
		return out << format("({}, {})", p.first, p.second);
	}

	template <typename... Args>
	inline std::istream& operator>>(std::istream& in, tuple<Args...>& t) {
		[&]<size_t... I>(std::index_sequence<I...>) {
			((in >> std::get<I>(t)), ...);
		}(std::make_index_sequence<sizeof...(Args)>());
		return in;
	}

	template <typename T, typename... Args>
	inline std::ostream& operator<<(std::ostream& out, const tuple<T, Args...>& t) {
		[&]<size_t... I>(std::index_sequence<I...>) {
			out << "(" << std::get<0>(t), ((out << ", " << std::get<I + 1>(t)), ...), out << ")";
		}(std::make_index_sequence<sizeof...(Args)>());
		return out;
	}

	template <typename T>
	inline void read_array(auto&& it, size_t read_n) mr_noexcept {
		copy_n(istream_iterator<T>(cin), read_n, it);
	}

	inline void put_array(auto&& begin, auto&& end, const char* sep = " ", bool end_line = true) mr_noexcept {
		auto pt = begin;
		cout << *pt++;
		while (pt != end) {
			cout << sep << *pt++;
		}
		if (end_line) cout << endl;
	}

	inline void put_array(auto&& r, const char* sep = " ") mr_noexcept {
		put_array(std::begin(r), std::end(r), sep);
	}

	template <typename... Args>
	struct __input_iterator_t {
		inline decltype(auto) operator++() mr_noexcept { return *this; }
		inline auto operator++(int) mr_noexcept { return *this; }
		inline auto operator*() const mr_noexcept {
			tuple<Args...> res;
			cin >> res;
			return res;
		}
	};

	template <typename... Args>
	constexpr __input_iterator_t<Args...> input_iterator;
} // namespace mirai