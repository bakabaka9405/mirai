#include <mirai/pch.hpp>
#include <mirai/math/math_theory_basic.hpp>
MR_NAMESPACE_BEGIN
ll bsgs(ll a, ll n, ll p) mr_noexcept {
	// a^x = n (mod p)
	a %= p, n %= p;
	std::unordered_map<ll, ll> giant;
	ll m = std::ceil(std::sqrt(p));
	for (ll i = 0, j = 1; i <= m; i++, j = j * a % p) giant[j] = i;
	ll powa = fastpow(a, m, p);
	for (ll i = 0, j = inv(n, p); i <= m; i++, j = j * powa % p) {
		if (giant.contains(j)) return i * m - giant[j];
	}
	return -1;
}
MR_NAMESPACE_END