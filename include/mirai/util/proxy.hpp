#pragma once
#include <mirai/pch.hpp>
namespace mirai {
	template <typename proxy_type, typename T>
	concept proxy_concept = requires() {
		proxy_type::work(T(), T());
	};

	template <typename T>
	struct default_add_proxy {
		inline static T work(const T& lt, const T& rt) {
			return lt + rt;
		}
	};

	template <typename T, size_t P>
		requires std::integral<T>
	struct mod_add_proxy {
		inline static T work(const T& lt, const T& rt) {
			static_assert(P > 0);
			return (lt + rt) % P;
		}
	};

	template <typename T, size_t P>
		requires std::integral<T>
	struct safe_mod_add_proxy {
		inline static T work(const T& lt, const T& rt) {
			static_assert(P > 0);
			return (lt + rt + P) % P;
		}
	};

	template <typename T, size_t P>
		requires std::integral<T>
	struct mod_mul_proxy {
		inline static T work(const T& lt, const T& rt) {
			static_assert(P > 0);
			return lt * rt % P;
		}
	};

	

} // namespace mirai