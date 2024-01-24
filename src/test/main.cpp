#include <mirai/graph/graph.hpp>
#include <mirai/util/tuple.hpp>
#include <mirai/util/io.hpp>
#include <mirai/ds/disjoint_set_union.hpp>
#include <mirai/graph/iterate.hpp>
#include <mirai/util/type_info.hpp>
#include <mirai/util/time.hpp>
#include <mirai/util/range.hpp>
#include <mirai/util/random.hpp>
using namespace mirai;
int main() {
	vector<ull> vec(1e8);
	vector<ull> cnt(200);
	ranges::generate(vec,__e);
	auto_timer t;
	ull res=0;
	for(auto i:vec)cnt[i%200]++;
	for(auto i:cnt)cout<<i<<endl;
	return 0;
}