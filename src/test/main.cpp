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
graph<void> G(100);
ll id[100], od[100];
vector<ll> euler;
struct _config {
	CONFIG_ITEM euler_order = euler;
} config;
int main() {
	G.insert(1, 2);
	G.insert(2, 3);
	G.insert(1, 4);
	dfs_in_tree<G,config>(1);
	for(auto i:euler)cout<<i<<endl;
	return 0;
}