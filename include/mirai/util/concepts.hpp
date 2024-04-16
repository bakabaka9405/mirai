#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
template <typename T>
concept vec2_like = requires(T&& t) {
	t.x;
	t.y;
};

template <typename T>
concept vec3_like = requires(T&& t) {
	t.x;
	t.y;
	t.z;
};

template <typename proxy_type, typename T>
concept proxy = requires() {
	proxy_type::work;
};

template <typename T>
concept arithmetic = std::is_arithmetic_v<T>;

template <typename T>
concept range = requires(T&& t) {
	mr_begin(t);
	mr_end(t);
};
MR_NAMESPACE_END