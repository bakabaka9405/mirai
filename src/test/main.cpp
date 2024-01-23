#include <mirai/graph/graph.hpp>
#include <mirai/util/tuple.hpp>
#include <mirai/util/io.hpp>
#include <mirai/ds/disjoint_set_union.hpp>
#include <mirai/graph/iterate.hpp>
#include <mirai/util/type_info.hpp>
#include <mirai/util/time.hpp>
using namespace mirai;
graph<ll, link_model> G(100);
int main() {
	auto_timer t;
	G.insert(1, 2,3);
	G.insert(1, 3,4);
	vector<pair<ll,ll>> vec;
	copy(G[1].begin(),G[1].end(), back_inserter(vec));
	put_array(vec,", ");
	return 0;
}