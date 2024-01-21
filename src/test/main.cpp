#include <mirai/graph/graph.hpp>
#include <mirai/util/tuple.hpp>
using namespace mirai;
int main() {
	graph<ll, mirai::link_model> G(101);
	G.insert(1, 2, 3);
	G.insert(1, 3, 4);
	for (auto [i, j] : G[1]) cout << i << " " << j << endl;
	return 0;
}