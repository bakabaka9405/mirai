#pragma once
#include <mirai/pch.hpp>
namespace mirai {
	[[nodiscard]] inline string reversed_str(string s) mr_noexcept {
		std::reverse(s.begin(), s.end());
		return s;
	}

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

	MR_NODISCARD inline string itos(ll x, ll base) mr_noexcept {
		string res;
		constexpr char digit[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		do {
			res.push_back(digit[x % base]);
			x /= base;
		} while (x);
		std::reverse(res.begin(), res.end());
		return res;
	}

	MR_NODISCARD inline string toupper(string str) mr_noexcept {
		for (auto& i : str) i = std::toupper(i);
		return str;
	}
} // namespace mirai