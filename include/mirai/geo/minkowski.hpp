#pragma once
#include <mirai/pch.hpp>
#include <mirai/math/vec2.hpp>
MR_NAMESPACE_BEGIN
vector<vec2f> minkowski(const vector<vec2f>& hull_1, const vector<vec2f>& hull_2) {
	vector<vec2f> res(hull_1.size() + hull_2.size() + 1);
	vector<vec2f> s1(hull_1.size()), s2(hull_2.size());
	for (size_t i = 0; i < hull_1.size() - 1; i++) s1[i] = hull_1[i + 1] - hull_1[i];
	s1.back() = hull_1[0] - hull_1.back();
	for (size_t i = 0; i < hull_2.size() - 1; i++) s2[i] = hull_2[i + 1] - hull_2[i];
	s2.back() = hull_2[0] - hull_2.back();
	res[0] = hull_1[0] + hull_2[0];
	ull i = 0, j = 0, cnt = 0;
	while (i < s1.size() && j < s2.size()) {
		cnt++;
		if (cross_product(s1[i], s2[j]) >= 0)
			res[cnt] = res[cnt - 1] + s1[i++];
		else res[cnt] = res[cnt - 1] + s2[j++];
	}
	while (i < s1.size()) {
		cnt++;
		res[cnt] = res[cnt - 1] + s1[i++];
	}
	while (j < s2.size()) {
		cnt++;
		res[cnt] = res[cnt - 1] + s2[j++];
	}
	return res;
}
MR_NAMESPACE_END