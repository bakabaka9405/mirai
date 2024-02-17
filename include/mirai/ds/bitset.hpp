#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
	template <size_t size>
	class bitset {
	private:
		constexpr static ull blocks = ((size + 63) >> 6u) + 1;
		ull data[blocks]{};

	public:
		constexpr bitset() mr_noexcept = default;
		constexpr bitset(const bitset&) mr_noexcept = default;
		constexpr bitset(bitset&&) mr_noexcept = default;
		constexpr bitset& operator=(const bitset&) mr_noexcept = default;
		constexpr bitset& operator=(bitset&&) mr_noexcept = default;
		inline void set0(ull pos) mr_noexcept { data[pos >> 6u] &= ~(1ull << (pos & 63u)); }
		inline void set1(ull pos) mr_noexcept { data[pos >> 6u] |= 1ull << (pos & 63u); }
		inline void set(ull pos, bool val) mr_noexcept { val ? set1(pos) : set0(pos); }
		MR_NODISCARD inline bool test(ull pos) const mr_noexcept { return data[pos >> 6u] & (1ull << (pos & 63u)); }
		MR_NODISCARD inline bool operator[](ull pos) const mr_noexcept { return test(pos); }
		MR_NODISCARD inline bitset operator&(const bitset& rhs) const mr_noexcept {
			bitset res;
			for (ull i = 0; i < blocks; i++) res.data[i] = data[i] & rhs.data[i];
			return res;
		}
		MR_NODISCARD inline bitset operator|(const bitset& rhs) const mr_noexcept {
			bitset res;
			for (ull i = 0; i < blocks; i++) res.data[i] = data[i] | rhs.data[i];
			return res;
		}
		MR_NODISCARD inline bitset operator^(const bitset& rhs) const mr_noexcept {
			bitset res;
			for (ull i = 0; i < blocks; i++) res.data[i] = data[i] ^ rhs.data[i];
			return res;
		}
		MR_NODISCARD inline bitset operator~() const mr_noexcept {
			bitset res;
			for (ull i = 0; i < blocks; i++) res.data[i] = ~data[i];
			return res;
		}
		constexpr bitset& operator&=(const bitset& rhs) mr_noexcept {
			for (ull i = 0; i < blocks; i++) data[i] &= rhs.data[i];
			return *this;
		}
		constexpr bitset& operator|=(const bitset& rhs) mr_noexcept {
			for (ull i = 0; i < blocks; i++) data[i] |= rhs.data[i];
			return *this;
		}
		constexpr bitset& operator^=(const bitset& rhs) mr_noexcept {
			for (ull i = 0; i < blocks; i++) data[i] ^= rhs.data[i];
			return *this;
		}
		MR_NODISCARD constexpr ull find_first() const mr_noexcept {
			for (ull i = 0; i < blocks; i++) {
				if (data[i]) return (i << 6u) + std::countl_zero(data[i]);
			}
			return size;
		}
		MR_NODISCARD constexpr ull find_next(ull pos) const mr_noexcept {
			ull block = pos >> 6u;
			ull offset = pos & 63u;
			ull mask = ~0ull << offset;
			if (data[block] & mask) return (block << 6u) + std::countl_zero(data[block] & mask);
			for (ull i = block + 1; i < blocks; i++) {
				if (data[i]) return (i << 6u) + std::countl_zero(data[i]);
			}
			return size;
		}
		MR_NODISCARD constexpr ull count() const mr_noexcept {
			ull res = 0;
			for (auto v:data) res += popcount(v);
			return res;
		}
		MR_NODISCARD constexpr bool all() const mr_noexcept {
			return ranges::all_of(data, [](ull v) { return v; });
		}
		MR_NODISCARD constexpr bool any() const mr_noexcept {
			return ranges::any_of(data, [](ull v) { return v; });
		}
		MR_NODISCARD constexpr bool none() const mr_noexcept {
			return ranges::none_of(data, [](ull v) { return v; });
		}
		MR_NODISCARD constexpr bool operator==(const bitset& rhs) const mr_noexcept {
			return ranges::equal(data, rhs.data);
		}
		MR_NODISCARD constexpr bool operator!=(const bitset& rhs) const mr_noexcept {
			return !(*this == rhs);
		}
		
	};
MR_NAMESPACE_END