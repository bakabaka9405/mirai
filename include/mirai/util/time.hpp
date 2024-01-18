#pragma once
#include <mirai/pch.hpp>
namespace mirai {
	MR_NODISCARD inline time_point<steady_clock> curr_time_point() mr_noexcept {
		return steady_clock::now();
	}

	MR_NODISCARD inline ll time_now() mr_noexcept {
		return curr_time_point().time_since_epoch().count();
	}

	struct auto_timer {
		time_point<steady_clock> start;
		auto_timer() mr_noexcept : start{ curr_time_point() } {}
		void restart() mr_noexcept { start = time_point<steady_clock>{ curr_time_point() }; }
		auto now() const mr_noexcept {
			return curr_time_point() - start;
		}
		~auto_timer() mr_noexcept {
			clog << format("timer stopped after {}.\n",
						   duration_cast<chrono::milliseconds>(curr_time_point() - start));
		}
	};

} // namespace mirai