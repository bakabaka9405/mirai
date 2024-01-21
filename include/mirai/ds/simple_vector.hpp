#pragma once
#include <mirai/pch.hpp>
namespace mirai {
	template <typename T>
	class simple_vector {
	private:
		size_t n;
		size_t capacity;
		T* arr;

	public:
		using iterator = T*;
		simple_vector() mr_noexcept : n{}, capacity(32), arr(new T[32]) {}
		inline void push_back(const T& val) {
			if (n == capacity)
				arr = (T*)realloc(arr, sizeof(T) * (capacity = capacity << 1 | 1));
			arr[n++] = val;
		}
		inline void pop_back() { n--; }
		inline T& operator[](size_t _index) {
			return arr[_index];
		}
		inline iterator begin() const { return arr; }
		inline iterator end() const { return arr + n; }
	};
} // namespace mirai