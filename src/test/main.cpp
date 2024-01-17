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
	auto t=read_tuple<int,double>();
	cout<<get_type_name(t)<<endl;
	return 0;
}