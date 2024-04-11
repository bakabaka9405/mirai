#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/range.hpp>
MR_NAMESPACE_BEGIN
template <size_t radix = 8>
void radix_sort(std::random_access_iterator auto begin, std::random_access_iterator auto end) {
	using T = std::decay_t<decltype(*begin)>;
	constexpr static size_t element_size = sizeof(T) * 8;
	static_assert(element_size % radix == 0, "radix must be a factor of element_size");
	size_t element_count = std::distance(begin, end);
	constexpr static size_t loop_times = element_size / radix;
	constexpr static size_t bucket_size = 1ull << radix;
	std::vector<T> buffer1(begin, end), buffer2(element_count);
	std::array<ull, bucket_size> bucket;
#pragma unroll
	for (ull i = 0; i < loop_times; i++) {
		bucket.fill(0);
		for (auto&& j : buffer1)
			bucket[(j >> (i * radix)) & (bucket_size - 1)]++;
		for (size_t j = 1; j < bucket_size; j++)
			bucket[j] += bucket[j - 1];
		for (auto&& j : buffer1 | views::reverse)
			buffer2[--bucket[(j >> (i * radix)) & (bucket_size - 1)]] = j;
		std::swap(buffer1, buffer2);
	}
	std::copy(buffer1.begin(), buffer1.end(), begin);
}

template <range _range, size_t radix = 8>
void radix_sort(_range&& rg) {
	radix_sort<radix>(mr_begin(rg), mr_end(rg));
}
MR_NAMESPACE_END