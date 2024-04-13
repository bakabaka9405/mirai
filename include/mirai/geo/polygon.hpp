#pragma once
#include <mirai/pch.hpp>
#include <mirai/math/vec2.hpp>
MR_NAMESPACE_BEGIN
double polygon_perimeter(const vector<vec2f>& poly) {
	double res = 0;
	for (size_t i = 0; i < poly.size(); i++)
		res += poly[i].distance_to(poly[(i + 1) % poly.size()]);
	return res;
}
double polygon_area(const vector<vec2f>& poly) {
	double res = 0;
	for (size_t i = 0; i < poly.size(); i++)
		res += cross_product(poly[i], poly[(i + 1) % poly.size()]);
	return std::abs(res / 2);
}
template <typename T>
double triangle_area(const vec2<T>& a, const vec2<T>& b, const vec2<T>& c) {
	return std::abs(cross_product(b - a, c - a) / 2.0);
}
MR_NAMESPACE_END