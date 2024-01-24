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
ll id[100];
struct{
	ll (&in_degree)[];
}config{id};
int main() {
	G.insert(1,2);
	G.insert(2,3);
	calc_graph_degree<G, config>();
	for(auto i:id)cout<<i<<endl;
	return 0;
}