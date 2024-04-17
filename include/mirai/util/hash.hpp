#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
namespace detail::wyhash {
	inline void mum(uint64_t* a, uint64_t* b) {
#if MR_HAVE_INT128
		__uint128_t r = *a;
		r *= *b;
		*a = static_cast<uint64_t>(r);
		*b = static_cast<uint64_t>(r >> 64U);
#elif defined(_MSC_VER) && defined(_M_X64)
		*a = _umul128(*a, *b, b);
#else
		uint64_t ha = *a >> 32U;
		uint64_t hb = *b >> 32U;
		uint64_t la = static_cast<uint32_t>(*a);
		uint64_t lb = static_cast<uint32_t>(*b);
		uint64_t hi{};
		uint64_t lo{};
		uint64_t rh = ha * hb;
		uint64_t rm0 = ha * lb;
		uint64_t rm1 = hb * la;
		uint64_t rl = la * lb;
		uint64_t t = rl + (rm0 << 32U);
		auto c = static_cast<uint64_t>(t < rl);
		lo = t + (rm1 << 32U);
		c += static_cast<uint64_t>(lo < t);
		hi = rh + (rm0 >> 32U) + (rm1 >> 32U) + c;
		*a = lo;
		*b = hi;
#endif
	}
	[[nodiscard]] inline auto mix(uint64_t a, uint64_t b) -> uint64_t {
		mum(&a, &b);
		return a ^ b;
	}
	[[nodiscard]] inline auto r8(const uint8_t* p) -> uint64_t {
		uint64_t v{};
		std::memcpy(&v, p, 8U);
		return v;
	}
	[[nodiscard]] inline auto r4(const uint8_t* p) -> uint64_t {
		uint32_t v{};
		std::memcpy(&v, p, 4);
		return v;
	}
	[[nodiscard]] inline auto r3(const uint8_t* p, size_t k) -> uint64_t {
		return (static_cast<uint64_t>(p[0]) << 16U) | (static_cast<uint64_t>(p[k >> 1U]) << 8U) | p[k - 1];
	}
	[[maybe_unused]] [[nodiscard]] inline auto hash(void const* key, size_t len) -> uint64_t {
		static constexpr auto secret = std::array{ UINT64_C(0xa0761d6478bd642f),
												   UINT64_C(0xe7037ed1a0b428db),
												   UINT64_C(0x8ebc6af09c88c6e3),
												   UINT64_C(0x589965cc75374cc3) };
		auto const* p = static_cast<uint8_t const*>(key);
		uint64_t seed = secret[0];
		uint64_t a{};
		uint64_t b{};
		if (MR_EXPECT(len <= 16)) {
			if (MR_EXPECT(len >= 4)) {
				a = (r4(p) << 32U) | r4(p + ((len >> 3U) << 2U));
				b = (r4(p + len - 4) << 32U) | r4(p + len - 4 - ((len >> 3U) << 2U));
			}
			else if (MR_EXPECT(len > 0)) {
				a = r3(p, len);
				b = 0;
			}
			else {
				a = 0;
				b = 0;
			}
		}
		else {
			size_t i = len;
			if (MR_UNEXPECT(i > 48)) {
				uint64_t see1 = seed;
				uint64_t see2 = seed;
				do {
						seed = mix(r8(p) ^ secret[1], r8(p + 8) ^ seed);
						see1 = mix(r8(p + 16) ^ secret[2], r8(p + 24) ^ see1);
						see2 = mix(r8(p + 32) ^ secret[3], r8(p + 40) ^ see2);
						p += 48;
						i -= 48;
				} while (MR_EXPECT(i > 48));
				seed ^= see1 ^ see2;
			}
			while (MR_UNEXPECT(i > 16)) {
				seed = mix(r8(p) ^ secret[1], r8(p + 8) ^ seed);
				i -= 16;
				p += 16;
			}
			a = r8(p + i - 16);
			b = r8(p + i - 8);
		}
		return mix(secret[1] ^ len, mix(a ^ secret[1], b ^ seed));
	}
	MR_NODISCARD inline auto hash(uint64_t x) -> uint64_t {
		return detail::wyhash::mix(x, UINT64_C(0x9E3779B97F4A7C15));
	}
} // namespace detail::wyhash
template <typename T, typename Enable = void>
struct hash {
	auto operator()(T const& obj) const noexcept(noexcept(std::declval<std::hash<T>>().operator()(std::declval<T const&>())))
		-> uint64_t {
		return std::hash<T>{}(obj);
	}
};
template <typename CharT>
struct hash<std::basic_string<CharT>> {
	using is_avalanching = void;
	auto operator()(std::basic_string<CharT> const& str) const noexcept -> uint64_t {
		return detail::wyhash::hash(str.data(), sizeof(CharT) * str.size());
	}
};
template <typename CharT>
struct hash<std::basic_string_view<CharT>> {
	using is_avalanching = void;
	auto operator()(std::basic_string_view<CharT> const& sv) const noexcept -> uint64_t {
		return detail::wyhash::hash(sv.data(), sizeof(CharT) * sv.size());
	}
};
template <class T>
struct hash<T*> {
	using is_avalanching = void;
	auto operator()(T* ptr) const noexcept -> uint64_t {
		return detail::wyhash::hash(reinterpret_cast<uintptr_t>(ptr));
	}
};
template <class T>
struct hash<std::unique_ptr<T>> {
	using is_avalanching = void;
	auto operator()(std::unique_ptr<T> const& ptr) const noexcept -> uint64_t {
		return detail::wyhash::hash(reinterpret_cast<uintptr_t>(ptr.get()));
	}
};
template <class T>
struct hash<std::shared_ptr<T>> {
	using is_avalanching = void;
	auto operator()(std::shared_ptr<T> const& ptr) const noexcept -> uint64_t {
		return detail::wyhash::hash(reinterpret_cast<uintptr_t>(ptr.get()));
	}
};
template <typename Enum>
struct hash<Enum, typename std::enable_if<std::is_enum<Enum>::value>::type> {
	using is_avalanching = void;
	auto operator()(Enum e) const noexcept -> uint64_t {
		using underlying = typename std::underlying_type_t<Enum>;
		return detail::wyhash::hash(static_cast<underlying>(e));
	}
};
template <typename... Args>
struct tuple_hash_helper {
	template <typename Arg>
	[[nodiscard]] constexpr static auto to64(Arg const& arg) -> uint64_t {
		if constexpr (std::is_integral_v<Arg> || std::is_enum_v<Arg>) {
			return static_cast<uint64_t>(arg);
		}
		else {
			return hash<Arg>{}(arg);
		}
	}
	[[nodiscard]] static auto mix64(uint64_t state, uint64_t v) -> uint64_t {
		return detail::wyhash::mix(state + v, uint64_t{ 0x9ddfea08eb382d69 });
	}
	template <typename T, std::size_t... Idx>
	[[nodiscard]] static auto calc_hash(T const& t, std::index_sequence<Idx...>) noexcept -> uint64_t {
		auto h = uint64_t{};
		((h = mix64(h, to64(std::get<Idx>(t)))), ...);
		return h;
	}
};
template <typename... Args>
struct hash<std::tuple<Args...>> : tuple_hash_helper<Args...> {
	using is_avalanching = void;
	auto operator()(std::tuple<Args...> const& t) const noexcept -> uint64_t {
		return tuple_hash_helper<Args...>::calc_hash(t, std::index_sequence_for<Args...>{});
	}
};
template <typename A, typename B>
struct hash<std::pair<A, B>> : tuple_hash_helper<A, B> {
	using is_avalanching = void;
	auto operator()(std::pair<A, B> const& t) const noexcept -> uint64_t {
		return tuple_hash_helper<A, B>::calc_hash(t, std::index_sequence_for<A, B>{});
	}
};
#define MR_HASH_STATICCAST(T)                                        \
	template <>                                                      \
	struct hash<T> {                                                 \
		using is_avalanching = void;                                 \
		auto operator()(T const& obj) const noexcept -> uint64_t {   \
			return detail::wyhash::hash(static_cast<uint64_t>(obj)); \
		}                                                            \
	}
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wuseless-cast"
#endif
MR_HASH_STATICCAST(bool);
MR_HASH_STATICCAST(char);
MR_HASH_STATICCAST(signed char);
MR_HASH_STATICCAST(unsigned char);
MR_HASH_STATICCAST(char8_t);
MR_HASH_STATICCAST(char16_t);
MR_HASH_STATICCAST(char32_t);
MR_HASH_STATICCAST(wchar_t);
MR_HASH_STATICCAST(short);
MR_HASH_STATICCAST(unsigned short);
MR_HASH_STATICCAST(int);
MR_HASH_STATICCAST(unsigned int);
MR_HASH_STATICCAST(long);
MR_HASH_STATICCAST(long long);
MR_HASH_STATICCAST(unsigned long);
MR_HASH_STATICCAST(unsigned long long);
#undef MR_HASH_STATICCAST
MR_NAMESPACE_END