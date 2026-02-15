#include <mirai/pch.hpp>

MR_NAMESPACE_BEGIN
vector<ll> prefix_function(const string& s) {
	vector<ll> border(s.length());
	for (ll i = 1, j = 0; i < (ll)s.length(); i++) {
		while (j && s[i] != s[j]) j = border[j - 1];
		if (s[i] == s[j]) j++;
		border[i] = j;
	}
	return border;
}

MR_NAMESPACE_END