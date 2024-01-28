#include <mirai/util/pipeline.hpp>
#include <mirai/util/time.hpp>
#include <mirai/util/debug.hpp>
#include <mirai/util/io.hpp>
using namespace mirai;
int main() {
	auto_timer __t;
	for (auto [i, j] : from(input_iterator<ll, ll>) | take(5)) debug(i, j);
	return 0;
}