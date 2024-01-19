#include <mirai/util/random.hpp>
#include <mirai/util/io.hpp>
#include <mirai/util/debug.hpp>
#include <mirai/math/vector.hpp>
#include <mirai/math/math_theory_basic.hpp>
#include <mirai/math/matrix.hpp>
#include <mirai/ds/sparse_table.hpp>
#include <mirai/util/string.hpp>
#include <mirai/util/range.hpp>
#include <mirai/util/time.hpp>
#include <mirai/util/type_info.hpp>
#include <mirai/ds/unordered_dense.hpp>
using namespace mirai;
constexpr ull n = 1e8;
vector<ull> v1(n),v2(n);
ull cnt;
int main() {
	ull *v3=new ull[n];
	srand(__gen());
	{
		auto_timer t;
		ranges::generate(v3,v3+n, fast_rand);
	}
	{
		auto_timer t;
		ranges::generate(v1, fast_rand);
	}
	for(ull i=0;i<n;i++)cnt+=(v1[i]>v3[i]);
	cout<<cnt<<endl;
	return 0;
}