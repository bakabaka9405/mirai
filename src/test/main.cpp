#include <mirai/graph/graph.hpp>
#include <mirai/graph/iterate.hpp>
#include <mirai/util/pipeline.hpp>
#include <mirai/util/io.hpp>
using namespace mirai;
graph<void> G;
ll fa[1000001], dep[1000001], sz[1000001], top[1000001], dfn[1000001];
constexpr struct _config1 {
	CONFIG_ITEM fa = ::fa;
	CONFIG_ITEM size = sz;
	CONFIG_ITEM depth = dep;
} config1;
constexpr struct _config2 {
	CONFIG_ITEM fa = ::fa;
	CONFIG_ITEM top = ::top;
	CONFIG_ITEM size = sz;
	CONFIG_ITEM dfn = ::dfn;
} config2;
ll n, m, s;
ll lca(ll u, ll v) {
	while (top[u] != top[v]) {
		if (dep[top[u]] > dep[top[v]])
			u = fa[top[u]];
		else
			v = fa[top[v]];
	}
	return dep[u] > dep[v] ? v : u;
}
int main() {
	n = readi(), m = readi(), s = readi();
	G.resize(n + 1);
	for (ll i = 1, u, v; i < n; i++) {
		u = readi(), v = readi();
		G.insert(u, v);
		G.insert(v, u);
	}
	dfs_in_tree<G, config1>(s);
	tree_decomposition<G, config2>(s);
	for (ll i = 1, u, v; i <= m; i++) {
		u = readi(), v = readi();
		puti(lca(u, v));
		puts("");
	}
	return 0;
}