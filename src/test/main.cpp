#include <mirai/util/random.hpp>
#include <mirai/util/io.hpp>
#include <mirai/util/debug.hpp>
#include <mirai/math/vector.hpp>
#include <mirai/math/math_theory_basic.hpp>
#include<mirai/math/matrix.hpp>
using namespace mirai;
int main() {
	using mat1=matrix<ll,2,2,mod_add_proxy<ll, 998244353>>;
	using mat2=matrix<ll,2,3,mod_add_proxy<ll, 998244353>>;
	mat1 a{1,2,3,4};
	mat2 b{4,3,2,1,0,-1};
	cout<<a*b<<endl;
	return 0;
}