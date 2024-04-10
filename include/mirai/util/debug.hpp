#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
template <typename... Args>
MR_API void print_dbg(Args... args) {
	if constexpr (sizeof...(Args) > 0)
		((cout << args << " "), ...) << endl;
	else
		cout << endl;
}

#ifdef MR_DEBUG
#define debug(...) mirai::print_dbg("[", __func__, ":", __LINE__, "]", ##__VA_ARGS__) // NOLINT(bugprone-lambda-function-name)
#else
#define debug(...)
#endif
MR_NAMESPACE_END