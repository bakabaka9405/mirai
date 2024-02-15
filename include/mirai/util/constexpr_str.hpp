#pragma once
#include <mirai/pch.hpp>
namespace mirai {
	enum { StaticStringLiteral,
		   StaticStringConcat };

	template <size_t N, int type>
	struct constexpr_str;

	template <size_t N>
	struct constexpr_str<N, StaticStringLiteral> {
		explicit constexpr constexpr_str(const char (&s)[N + 1])
			: data(s) {}
		explicit constexpr operator string_view() const mr_noexcept {
			return data;
		}
		const char (&data)[N + 1];
	};

	template <size_t N>
	struct constexpr_str<N, StaticStringConcat> {
		template <size_t N1, int T1, size_t N2, int T2>
		explicit constexpr constexpr_str(const constexpr_str<N1, T1>& s1,
										 const constexpr_str<N2, T2>& s2)
			: constexpr_str(s1,
							std::make_index_sequence<N1>{},
							s2,
							std::make_index_sequence<N2>{}) {}

		template <size_t N1, int T1, size_t... I1, size_t N2, int T2, size_t... I2>
		constexpr constexpr_str(const constexpr_str<N1, T1>& s1,
								std::index_sequence<I1...>,
								const constexpr_str<N2, T2>& s2,
								std::index_sequence<I2...>)
			: data{ s1.data[I1]..., s2.data[I2]..., '\0' } {
			static_assert(N == N1 + N2, "static_string length error");
		}
		explicit constexpr operator string_view() const mr_noexcept {
			return data;
		}
		char data[N + 1]{};
	};

	template <size_t N>
	constexpr auto string_literal(const char (&s)[N]) {
		return constexpr_str<N - 1, StaticStringLiteral>(s);
	}

	template <size_t N, int T>
	constexpr auto string_literal(constexpr_str<N, T> s) {
		return s;
	}

	template <size_t N1, int T1, size_t N2, int T2>
	constexpr auto string_concat(const constexpr_str<N1, T1>& s1,
								 const constexpr_str<N2, T2>& s2) mr_noexcept {
		return constexpr_str<N1 + N2, StaticStringConcat>(s1, s2);
	}

	template <size_t N1, int T1, size_t N2, int T2, typename... Args>
	constexpr auto string_concat(const constexpr_str<N1, T1>& s1,
								 const constexpr_str<N2, T2>& s2,
								 Args&&... args) mr_noexcept {
		return string_concat(string_concat(s1, s2), args...);
	}

	template <size_t N1, int T1, size_t N2>
	constexpr auto string_concat(const constexpr_str<N1, T1>& s1,
								 const char (&s2)[N2]) mr_noexcept {
		return string_concat(s1, string_literal(s2));
	}

	template <size_t N1, int T1, size_t N2, typename... Args>
	constexpr auto string_concat(const constexpr_str<N1, T1>& s1,
								 const char (&s2)[N2],
								 Args&&... args) mr_noexcept {
		return string_concat(string_concat(s1, string_literal(s2)), args...);
	}

	template <size_t N1, size_t N2, int T2>
	constexpr auto string_concat(const char (&s1)[N1],
								 const constexpr_str<N2, T2>& s2) mr_noexcept {
		return string_concat(string_literal(s1), s2);
	}

	template <size_t N1, size_t N2, int T2, typename... Args>
	constexpr auto string_concat(const char (&s1)[N1],
								 const constexpr_str<N2, T2>& s2,
								 Args&&... args) mr_noexcept {
		return string_concat(string_concat(string_literal(s1), s2), args...);
	}

	template <size_t N1, size_t N2>
	constexpr auto string_concat(const char (&s1)[N1],
								 const char (&s2)[N2]) mr_noexcept {
		return string_concat(string_literal(s1), string_literal(s2));
	}

	template <size_t N1, size_t N2, typename... Args>
	constexpr auto string_concat(const char (&s1)[N1],
								 const char (&s2)[N2],
								 Args&&... args) mr_noexcept {
		return string_concat(string_concat(string_literal(s1), string_literal(s2)), args...);
	}

	template <size_t N, int T>
	std::ostream& operator<<(std::ostream& out, const constexpr_str<N, T>& s) mr_noexcept {
		return out << s.data;
	}
} // namespace mirai