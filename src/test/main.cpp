#include <mirai/graph/graph.hpp>
#include <mirai/util/tuple.hpp>
#include <mirai/util/io.hpp>
#include <mirai/ds/disjoint_set_union.hpp>
#include <mirai/graph/iterate.hpp>
#include <mirai/util/type_info.hpp>
#include <mirai/util/time.hpp>
#include <mirai/util/range.hpp>
#include <mirai/util/random.hpp>
#include <mirai/util/pipeline.hpp>
#include <mirai/util/debug.hpp>
using namespace mirai;
graph<void, link_model> G;
ll id[100], od[100];
vector<ll> topo;
vector<ll> res;
struct _config {
	CONFIG_ITEM in_degree = id;
	CONFIG_ITEM topo_order = topo;
} config;
int main() {
	auto_timer t;
	G.resize(10);
	G.insert(2, 1);
	G.insert(2, 3);
	G.insert(1, 4);
	calc_graph_degree<G, config>();
	topo_sort<G, config, priority_queue<ll, vector<ll>, greater<ll>>>();
	for (auto i : topo) cout << i << " ";
	cout << endl;
	return 0;
}