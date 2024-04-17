#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
template <typename T>
class simple_vector {
private:
	size_t n;
	size_t capacity;
	T* arr;

public:
	using iterator = T*;
	constexpr simple_vector() mr_noexcept : n{}, capacity(32), arr((T*)malloc(sizeof(T) * 32)) {}
	constexpr ~simple_vector() mr_noexcept { free(arr); }
	inline void push_back(const T& val) mr_noexcept {
		if (n == capacity)
			arr = (T*)realloc(arr, sizeof(T) * (capacity = capacity << 1u | 1u));
		arr[n++] = val;
	}
	inline void pop_back() mr_noexcept { n--; }
	inline T& operator[](size_t _index) {
		return arr[_index];
	}
	inline iterator begin() mr_noexcept { return arr; }
	inline iterator end() mr_noexcept { return arr + n; }
};
MR_NAMESPACE_END