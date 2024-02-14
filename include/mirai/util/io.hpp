#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/string.hpp>
namespace mirai {

	MR_API void disable_stdio_sync() mr_noexcept {
		std::ios::sync_with_stdio(false);
		cin.tie(nullptr);
		cout.tie(nullptr);
	}

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
	MR_API ll readi() {
		ull x = 0;
		bool f = false;
		ull c = getchar_unlocked();
		while (c < '0' || c > '9') {
			if (c == '-') MR_UNLIKELY f = true;
			c = getchar_unlocked();
		}
		while (c >= '0' && c <= '9') {
			x = (x << 1u) + (x << 3u) + (c ^ 48u);
			c = getchar_unlocked();
		}
		return f ? static_cast<ll>(x) : x;
	}

	MR_API void puti(std::integral auto&& x) {
		if (x < 0) putchar_unlocked('-'), x = -x;
		if (x > 9) puti(x / 10);
		putchar_unlocked(x % 10 + '0');
	}

	template <typename... Args>
	MR_API void input(Args&&... args) {
		((cin >> args), ...);
	}

	template <typename... Args>
	MR_API void output(Args&&... args) {
		((cout << args), ...);
	}

	template <typename T, typename Y>
	MR_API std::istream& operator>>(std::istream& in, pair<T, Y>& p) {
		return in >> p.first >> p.second;
	}

	template <typename T, typename Y>
	MR_API std::ostream& operator<<(std::ostream& out, const pair<T, Y>& p) {
		return out << format("({}, {})", p.first, p.second);
	}

	template <typename... Args>
	MR_API std::istream& operator>>(std::istream& in, tuple<Args...>& t) {
		[&]<size_t... I>(std::index_sequence<I...>) {
			((in >> std::get<I>(t)), ...);
		}(std::make_index_sequence<sizeof...(Args)>());
		return in;
	}

	template <typename T, typename... Args>
	MR_API std::ostream& operator<<(std::ostream& out, const tuple<T, Args...>& t) {
		[&]<size_t... I>(std::index_sequence<I...>) {
			out << "(" << std::get<0>(t), ((out << ", " << std::get<I + 1>(t)), ...), out << ")";
		}(std::make_index_sequence<sizeof...(Args)>());
		return out;
	}

	template <typename T>
	MR_API void read_array(auto&& it, size_t read_n) mr_noexcept {
		copy_n(istream_iterator<T>(cin), read_n, it);
	}

	MR_API void put_array(auto&& begin, auto&& end, const char* sep = " ", bool end_line = true) mr_noexcept {
		auto pt = begin;
		cout << *pt++;
		while (pt != end) {
			cout << sep << *pt++;
		}
		if (end_line) cout << endl;
	}

	MR_API void put_array(auto&& r, const char* sep = " ") mr_noexcept {
		put_array(std::begin(r), std::end(r), sep);
	}

	template <typename T, typename... Args>
	struct __scanner_t { // NOLINT
		inline decltype(auto) operator++() mr_noexcept { return *this; }
		inline auto operator++(int) mr_noexcept->__scanner_t { return *this; }
		inline auto get() const mr_noexcept {
			std::conditional_t<sizeof...(Args) == 0, T, tuple<T, Args...>> res;
			cin >> res;
			return res;
		}
		inline auto operator*() const mr_noexcept { return get(); }
	};

	template <typename... Args>
	constexpr __scanner_t<Args...> scanner;
} // namespace mirai