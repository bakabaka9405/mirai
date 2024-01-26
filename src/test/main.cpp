#include <mirai/util/pipeline.hpp>
using namespace mirai;
int main() {
	vector v{ 1, 9, 2, 4, 6, -8, -10, 7, 3, 5 };
	auto i = v | skip(2) | transform(static_cast<double (*)(double)>(abs)) | maximum<ll> | endp;
	cout << i << endl;
}