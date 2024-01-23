#include <iterator>
#include <mirai/graph/graph_model.hpp>
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
	constexpr static struct {
	} config;
	struct s {
		ll v;
	};
	G.insert(1, 2, 3);
	G.insert(1, 3, 4);
	G.insert(1, 4, 5);
	auto r=G[1];
	cout<<get_type_name(r)<<endl;
	return 0;
}