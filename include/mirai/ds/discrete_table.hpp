#pragma once
#include <mirai/pch.hpp>
namespace mirai {
	template <typename T, typename Map = std::map<T, ull>, typename Buffer = std::vector<T>>
	class static_discrete_table {
	private:
		Map _map;
		Buffer _buf;

	public:
		static_discrete_table() mr_noexcept = default;
		inline void insert(const T& t) mr_noexcept { _map[t] = 0; }
		inline bool contains(const T& t) const mr_noexcept { return _map.contains(t); }
		void settle() mr_noexcept {
			_buf.resize(_map.size());
			for (ull t = 0; auto& [i, j] : _map) _buf[j = t++] = i;
		}
		ull operator[](const T& t) const mr_noexcept { return _map[t]; }
		T original(size_t index) { return _buf[index]; }
	};

	template <typename T, typename Map = std::map<T, ull>>
	class dynamic_discrete_table {
	private:
		Map _map;

	public:
		dynamic_discrete_table() mr_noexcept = default;
		inline bool contains(const T& t) const mr_noexcept { return _map.contains(t); }
		inline void insert(const T& t) mr_noexcept { _map.insert({ t, _map.size() }); }
		ull operator[](const T& t) const mr_noexcept { return _map[t]; }
		T original(size_t index) { return _map.lower_bound(index).first; }
	};
} // namespace mirai
