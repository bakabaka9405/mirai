#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
template <size_t N, size_t... Args>
constexpr auto cartesian_product() {
	if constexpr (sizeof...(Args) == 0) {
		auto res = std::array<std::array<size_t, 1>, N>();
		for (size_t i = 0; i < N; i++) {
			res[i][0] = i;
		}
		return res;
	}
	else {
		auto res = std::array<std::array<size_t, sizeof...(Args) + 1>, N * cartesian_product<Args...>().size()>();
		auto sub = cartesian_product<Args...>();
		for (size_t i = 0; i < N; i++) {
			for (size_t j = 0; j < sub.size(); j++) {
				res[i * sub.size() + j][0] = i;
				for (size_t k = 0; k < sub[j].size(); k++) {
					res[i * sub.size() + j][k + 1] = sub[j][k];
				}
			}
		}
		return res;
	}
}
MR_NAMESPACE_END