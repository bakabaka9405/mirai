#include <mirai/util/pipeline.hpp>
using namespace mirai;
int main() {
	vector v{ 1, 9, 2, 4, 6, 8, 10, 7, 3, 5 };
	auto i = v | skip(2) | transform([](int x) { return x * x - 2 * x; }) | maximum<ll>() | endp;
	cout << i << endl;
}