#include <mirai/graph/tree_decomposition.hpp>
#include <mirai/util/pipeline.hpp>
#include <mirai/util/io.hpp>
using namespace mirai;
graph<void> G;
ll fa[1000001], dep[1000001], sz[1000001], top[1000001];
constexpr struct _config1 {
	CONFIG_ITEM fa = ::fa;
	CONFIG_ITEM size = sz;
	CONFIG_ITEM depth = dep;
} config1;
constexpr struct _config2 {
	CONFIG_ITEM fa = ::fa;
	CONFIG_ITEM top = ::top;
	CONFIG_ITEM size = sz;
	CONFIG_ITEM depth = dep;
} config2;
ll n, m, s;
int main() {
	disable_sync();
	input(n, m, s);
	G.resize(n + 1);
	for (auto [u, v] : from(input_iterator<ll, ll>) | take(n - 1)) {
		G.insert(u, v);
		G.insert(v, u);
	}
	dfs_in_tree<G, config1>(s);
	tree_decomposition<G, config2>(s);
	for (auto [u, v] : from(input_iterator<ll, ll>) | take(m)) {
		puti(tree_path_lca<config2>(u, v));
		puts("");
	}
	return 0;
}