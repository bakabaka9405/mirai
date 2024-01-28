#include <mirai/util/pipeline.hpp>
#include <mirai/util/io.hpp>
#include <mirai/math/kahan.hpp>
#include <mirai/misc/binary_search.hpp>
using namespace mirai;
ll n, m;
array<ll, 100001> a, b;
int main() {
	input(n, m);
	for (auto i : views::iota(1ll, n + 1)) cin >> a[i] >> b[i];
	if (m >= std::accumulate(a.begin() + 1, a.begin() + 1 + n, 0ll)) {
		cout << -1 << endl;
		return 0;
	}
	double ans = float_bsearch_by_times<dextral>(0, 1e10, 200, [&](double mid) {
		kahan sum;
		views::iota(1ll, n + 1) | transform([&](ll x) { return a[x] * mid - b[x]; }) | filter(is_positive) | addup_to(sum) | endp;
		return (*sum <= mid * m);
	});
	cout << std::fixed << setprecision(10) << ans << endl;
	return 0;
}