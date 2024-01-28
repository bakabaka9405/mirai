#include <mirai/util/pipeline.hpp>
using namespace mirai;
int main() {
	ll vec[]{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	for (auto i : from(vec + 1) | take(5)) cout << i << endl;
	return 0;
}