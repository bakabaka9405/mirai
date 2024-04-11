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
MR_NAMESPACE_END