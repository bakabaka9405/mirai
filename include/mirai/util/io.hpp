#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/string.hpp>
#include <mirai/util/pipe.hpp>
MR_NAMESPACE_BEGIN
MR_API void disable_stdio_sync() mr_noexcept {
	std::ios::sync_with_stdio(false);
	cin.tie(nullptr);
	cout.tie(nullptr);
}

MR_API void lock_stdio() mr_noexcept {
	flockfile(stdin);
	flockfile(stdout);
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
MR_API ll readi() mr_noexcept {
	ull x = 0;
	bool f = false;
	int c = getchar_unlocked();
	while (!isdigit(c) && c != '-') c = getchar_unlocked();
	if (c == '-') MR_UNLIKELY {
			f = true;
			c = getchar_unlocked();
		}
	while (isdigit(c)) {
		x = (x << 1u) + (x << 3u) + (static_cast<ull>(c) ^ 48u);
		c = getchar_unlocked();
	}
	return f ? -x : x; // NOLINT
}

MR_API ull readu() mr_noexcept {
	ull x = 0;
	int c = getchar_unlocked();
	while (!isdigit(c)) c = getchar_unlocked();
	while (isdigit(c)) {
		x = (x << 1u) + (x << 3u) + (static_cast<ull>(c) ^ 48u);
		c = getchar_unlocked();
	}
	return x; // NOLINT
}

MR_API ull readu_mod(ll P) mr_noexcept {
	ull x = 0;
	int c = getchar_unlocked();
	while (!isdigit(c)) c = getchar_unlocked();
	while (isdigit(c)) {
		x = (x << 1u) + (x << 3u) + (static_cast<ull>(c) ^ 48u);
		x %= P;
		c = getchar_unlocked();
	}
	return x; // NOLINT
}

MR_API void puti(std::integral auto x) mr_noexcept {
	constexpr static ull width = std::numeric_limits<std::decay_t<decltype(x)>>::digits10 + 3;
	static char buf[width];
	if (x < 0) {
		putchar_unlocked('-');
		x = -x;
	}
	ull p = width;
	do {
		buf[--p] = x % 10 + '0';
		x /= 10;
	} while (x != 0);
	fwrite_unlocked(buf + p, 1, width - p, stdout);
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
	return out << '(' << p.first << ", " << p.second << ')';
}

template <typename T, typename... Args>
MR_API std::ostream& operator<<(std::ostream& out, const tuple<T, Args...>& t) {
	[&]<size_t... I>(std::index_sequence<I...>) {
		out << "(" << std::get<0>(t), ((out << ", " << std::get<I + 1>(t)), ...), out << ")";
	}(std::make_index_sequence<sizeof...(Args)>());
	return out;
}

template <typename... Args>
MR_API std::istream& operator>>(std::istream& in, tuple<Args...>& t) {
	[&]<size_t... I>(std::index_sequence<I...>) {
		((in >> std::get<I>(t)), ...);
	}(std::make_index_sequence<sizeof...(Args)>());
	return in;
}

template <typename T, typename... Args>
auto __scan() {
	std::conditional_t<sizeof...(Args) == 0, T, tuple<T, Args...>> res;
	cin >> res;
	return res;
}

#define scan(...) __scan<__VA_ARGS__>()

template <typename T, typename... Args>
struct __scanner_t { // NOLINT
	inline decltype(auto) operator++() mr_noexcept { return *this; }
	inline auto operator++(int) mr_noexcept->__scanner_t { return *this; }
	inline static auto get() mr_noexcept {
		return scan(T, Args...);
	}
	inline auto operator*() const mr_noexcept { return get(); }
};

template <typename... Args>
constexpr __scanner_t<Args...> scanner;

template <typename... Args>
auto from_scanner = from(scanner<Args...>);

#define multicase            \
	ll case_count = scan(ll); \
	for (ll current_case = 1; current_case <= case_count; ++current_case)
MR_NAMESPACE_END