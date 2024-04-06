#pragma once
#include <mirai/pch.hpp>
namespace mirai {
	namespace detail {
		template <size_t N, size_t... Ns>
		constexpr size_t get_mul() {
			return N * (Ns * ...);
		}
	} // namespace detail

	template <typename T, size_t N, size_t... Ns>
	class md_array {
		constexpr static size_t _size = detail::get_mul<N, Ns...>();
		T data[_size];

	public:
		constexpr md_array() = default;
		
	};
} // namespace mirai