#pragma once
#include<mirai/pch.hpp>
namespace mirai{
	[[nodiscard]] inline 
	ll time_now()mr_noexcept{
		return chrono:: system_clock:: now().time_since_epoch().count();
	}
}