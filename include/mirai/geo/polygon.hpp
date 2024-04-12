#pragma once
#include <mirai/pch.hpp>
#include <mirai/math/vector.hpp>
MR_NAMESPACE_BEGIN
double polygon_perimeter(const vector<vec2f>& hull) {
	double res = 0;
	for (size_t i = 0; i < hull.size(); i++)
		res += hull[i].distance_to(hull[(i + 1) % hull.size()]);
	return res;
}
double polygon_area(const vector<vec2f>& polygon) {
	double res = 0;
	for (size_t i = 0; i < polygon.size(); i++)
		res += cross_product(polygon[i], polygon[(i + 1) % polygon.size()]);
	return std::abs(res / 2);
}
template <typename T>
double triangle_area(const vec2<T>& a, const vec2<T>& b, const vec2<T>& c) {
	return std::abs(cross_product(b - a, c - a) / 2.0);
}
MR_NAMESPACE_END