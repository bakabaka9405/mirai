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
graph<ll> G(100);
int main() {
	for (auto [i,j] : zip(vector{ 1, 2, 3, 4, 5 }, vector{ 2, 4, 6, 8, 10 }) | transform([](auto&& x) { return pair{x.second,x.first}; })) debug(i,j);
	return 0;
}