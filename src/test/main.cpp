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
using namespace mirai;
int main() {
	vector<ll> v1(1e7);
	rand_generator<ll> gen(1, 1e8);
	ll res=0;
	ranges::generate(v1, gen);
	{
		auto_timer timer;
		for(auto [i,j]:zip(views::iota(0ll,static_cast<ll>(v1.size())),v1)){
			res=max(res,i*2+j);
		}
	}
	cout<<res<<endl;
	return 0;
}