#include <mirai/pch.hpp>

MR_NAMESPACE_BEGIN
vector<ll> z_function(string s) {
	ll n = (ll)s.length();
	vector<ll> z(n);
	for (ll i = 1, l = 0, r = 0; i < n; ++i) {
		if (i <= r && z[i - l] < r - i + 1) z[i] = z[i - l];
		else {
			z[i] = max(0ll, r - i + 1);
			while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
		}
		if (i + z[i] - 1 > r) l = i, r = i + z[i] - 1;
	}
	return z;
}

MR_NAMESPACE_END