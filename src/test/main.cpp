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
using namespace mirai;
int main() {
	vector vec{1,2,3,4,5};
	for(auto [i,j]:enumerate(vec)){
		cout<<get_type_name(std::move(i),j)<<endl;
	}
	return 0;
}