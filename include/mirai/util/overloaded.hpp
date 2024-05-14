#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
template <class... Ts>
struct overloaded : Ts... {
	using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

MR_NAMESPACE_END