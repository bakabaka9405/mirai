#include <mirai/util/pipeline.hpp>
#include <mirai/util/debug.hpp>
#include <mirai/util/io.hpp>
using namespace mirai;
auto among(ll l, ll r) {
	return views::iota(l, r + 1);
}
array<ll, 100001> fa, dfn, lev, dep, top, sz, L, R, F, arr, sum, tag;
ll n, m, K, len;
void build() {
	len = pow(n, 0.4), K = (n + len - 1) / len;
	for (ll i : among(1, K)) L[i] = R[i - 1] + 1, R[i] = L[i] + len - 1;
	R[K] = n;
	for (ll i : among(1, K)) {
		for (ll j : among(L[i], R[i])) F[j] = i, sum[i] += arr[j];
	}
}
void add(ll l, ll r, ll v) {
	if (F[l] == F[r]) {
		for (ll i : among(l, r)) arr[i] += v;
		sum[F[l]] += v * (r - l + 1);
		return;
	}
	add(l, R[F[l]], v);
	add(L[F[r]], r, v);
	for (ll i : among(F[l] + 1, F[r] - 1)) tag[i] += v;
}
ll query(ll l, ll r) {
	ll res = 0;
	if (F[l] == F[r]) {
		among(l, r) | transform([](ll x) { return arr[x]; }) | addup_to(res) | endp;
		return res + tag[F[l]] * (r - l + 1);
	}
	res = query(l, R[F[l]]) + query(L[F[r]], r);
	among(F[l] + 1, F[r] - 1) | transform([](ll x) { return sum[x] + tag[x] * (R[x] - L[x] + 1); }) | addup_to(res) | endp;
	return res;
}
int main() {
	disable_stdio_sync();
	input(n);
	auto t = transform([]() {});
	from(input_iterator<ll>) | take(n) | save_to(arr.begin() + 1) | endp;
	build();
	for (auto [opt, l, r, c] : from(input_iterator<ll, ll, ll, ll>) | take(n)) {
		if (opt == 0)
			add(l, r, c);
		else
			cout << query(l, r) % (c + 1) << endl;
	}
	return 0;
}