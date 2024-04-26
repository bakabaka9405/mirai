# Mirai

> easy, fast modern C++ library for programming competition

This library provides all kinds of useful tools for programming competitors, including data structures, algorithm functions, debugging tools and so on. And it provides a tool to embed all the headers from this library into the single source file, which competitors can directly submit to the online judge.

## Install

First, you need to install Xmake.

To build the embedder tool, run the following command at the root of the library:

```bash
xmake -w embedder
```

After that, you can find the embedder tool named `embedder.exe` inside the `build` directory(also depending on the build mode you choose). Then you can add it to the path of environment variables.

## Example

Create a cpp file and add the Mirai library to the include search directories. For simplicity, you can directly write your code in `test/main.cpp`

Put these into the file:

```cpp
#include <mirai/ds/disjoint_set_union.hpp>
#include <mirai/util/io.hpp>
using namespace mirai;
struct vec {
	ll val = 0;
	vec operator+(const vec& rt) const {
		ll v = val + rt.val;
		if (v == 1) return { 1 };
		else if (v == -1) return { -1 };
		else if (v == 2) return { -1 };
		else if (v == -2) return { 1 };
		else return { 0 };
	}
	vec operator-() const { return { -val }; }
	bool operator==(const vec& rt) const { return val == rt.val; }
};
int main() {
	auto [n, q] = scan<ll, ll>();
	alias::dsu<vec> D(n + 1);
	ll ans = 0;
	for (auto [a, b, c] : from_scanner<ll, ll, ll> | take(q)) {
		if (b > n || c > n || (a == 2 && b == c)) ans++;
		else {
			if (a == 1) {
				if (!D.try_merge(b, c, vec{ 0 })) ans++;
			}
			else if (!D.try_merge(b, c, vec{ 1 })) ans++;
		}
	}
	cout << ans << endl;
	return 0;
}
```

And then, run

```bash
embedder.exe ./test.cpp <PATH_TO_MIRAI_INCLUDE_DIR> <PATH_TO_OUTPUT_DIR>
```

For example, run `embedder.exe ./test.cpp E:\\Working\\proj\\mirai\\include\\ C:\\Temp\\embed.cpp`

The embedder will scan your source file, then embed all the header file needed into it. A new file `embed.cpp` will be created, containing the parsed code. It will automatically opened by vscode (if installed). You can change the tool if you want.

The embedder only support full-English paths. Paths containing Chinese are not supported.

You can use the code to pass problem P2024 on Luogu, but be careful not to get banned. HAHA.