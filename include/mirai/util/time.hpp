#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
MR_NODISCARD MR_API time_point<steady_clock> curr_time_point() mr_noexcept {
	return steady_clock::now();
}

MR_NODISCARD MR_API ll time_now() mr_noexcept {
	return curr_time_point().time_since_epoch().count();
}

struct auto_timer {
	time_point<steady_clock> start;
	string name;
	explicit auto_timer(string name_ = "") mr_noexcept : start{ curr_time_point() }, name(std::move(name_)) {}
	void restart() mr_noexcept { start = time_point<steady_clock>{ curr_time_point() }; }
	MR_NODISCARD auto now() const mr_noexcept {
		return curr_time_point() - start;
	}
	~auto_timer() mr_noexcept {
		if (name.empty())
			cout << "timer stopped after " << duration_cast<chrono::milliseconds>(curr_time_point() - start) << "." << endl;
		else
			cout << "timer [" << name << "] stopped after " << duration_cast<chrono::milliseconds>(curr_time_point() - start) << "." << endl;
	}
};

MR_NAMESPACE_END