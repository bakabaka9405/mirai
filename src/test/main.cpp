#include <mirai/graph/graph.hpp>
#include <mirai/util/tuple.hpp>
#include <mirai/util/io.hpp>
#include <mirai/ds/disjoint_set_union.hpp>
#include <mirai/graph/iterate.hpp>
#include <mirai/util/type_info.hpp>
#include <mirai/util/time.hpp>
using namespace mirai;
graph<ll, vector_model> G(100);
ll fa[1000001], sz[1000001], dfn[1000001], lev[1000001], t;
int main() {
	auto_timer t;
	cout<<sizeof(G)<<endl;
	return 0;
}