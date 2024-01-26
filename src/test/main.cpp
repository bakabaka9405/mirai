#include <mirai/util/pipeline.hpp>
using namespace mirai;
int main() {
	vector v{ 1, -11, 2, 4, 6, -10, -9, 7, 3, 5 };
	auto i = v | skip(2) | filter([](int x) { return x % 2; }) | transform(as_abs) | maximum<ll> | endp;
	cout << i << endl;
}