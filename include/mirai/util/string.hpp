#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
[[nodiscard]] inline string reversed_str(string s) mr_noexcept {
	std::reverse(s.begin(), s.end());
	return s;
}
#if MR_HAVE_INT128
[[nodiscard]] inline i128 stoi128(const string& s) mr_noexcept {
	i128 res = 0, f = 1;
	auto pt = s.begin();
	if (*pt == '-') f = -1, pt++;
	while (pt != s.end()) {
		res = res * 10 + (*pt++ - '0');
	}
	return res * f;
}

[[nodiscard]] inline string to_string(i128 v) mr_noexcept {
	if (v == 0) return "0";
	string res;
	bool f = v < 0;
	if (f) v = -v;
	while (v) {
		res.push_back(v % 10 + '0');
	}
	if (f) res.push_back('-');
	return reversed_str(res);
}
#endif
MR_NODISCARD inline string itos(ll x, ll base, ll minimum_width = 0) mr_noexcept {
	string res;
	constexpr char digit[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	do {
		res.push_back(digit[x % base]);
		x /= base;
	} while (x);
	if (res.length() < minimum_width) res.append(string(minimum_width - res.length(), '0'));
	std::reverse(res.begin(), res.end());
	return res;
}

MR_NODISCARD inline string toupper(string str) mr_noexcept {
	for (auto& i : str) i = std::toupper(i);
	return str;
}

template <typename container>
container split(const std::string_view sv, const std::string_view delim = " ") {
	container ret;
	auto view = sv | views::split(delim) | views::transform([](auto&& rng) {
					return std::string_view(std::addressof(*rng.begin()), ranges::distance(rng));
				});
	auto ins = std::inserter(ret, ret.end());
	for (auto&& e : view)
		ins = { e.begin(), e.end() };
	return ret;
}
MR_NAMESPACE_END