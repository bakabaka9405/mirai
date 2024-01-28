#include <ios>
#include <mirai/graph/tree_decomposition.hpp>
#include <mirai/graph/iterate.hpp>
#include <mirai/util/pipeline.hpp>
#include <mirai/util/io.hpp>
#include <mirai/math/kahan.hpp>
using namespace mirai;
ll n, m;
array<ll, 100001> a, b;
int main() {
	input(n, m);
	for (auto i : views::iota(1ll, n + 1)) cin >> a[i] >> b[i];
	if (m >= std::accumulate(a.begin() + 1, a.begin() + 1 + n, 0)) {
		cout << -1 << endl;
		return 0;
	}
	double l = 0, r = 1e10, mid;
	for (auto _ : views::iota(200ll)) {
		mid = (l + r) / 2;
		kahan sum;
		for (auto i : views::iota(1ll, n + 1) | transform([&](ll x) { return a[x] * mid - b[x]; }) | filter(is_positive)) sum += i;
		if(*sum<=mid*m)l=mid;
		else r=mid;
	}
	cout<<std::fixed<<setprecision(10)<<l<<endl;
	return 0;
}