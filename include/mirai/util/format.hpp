#ifndef FMT_BASE_H_
#define FMT_BASE_H_
#define _tn typename
#define _tp template
#define _rt return
#define _sc static_cast
#define _cx constexpr
#define _cv
using ll = long long;
using ull = unsigned long long;
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <cstddef>
#include <type_traits>
#define FMT_VERSION 100202
#if defined(__clang__) && !defined(__ibmxl__)
#define FMT_CLANG_VERSION (__clang_major__ * 100 + __clang_minor__)
#else
#define FMT_CLANG_VERSION 0
#endif
#if defined(__GNUC__) && !defined(__clang__) && !defined(__INTEL_COMPILER)
#define FMT_GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)
#else
#define FMT_GCC_VERSION 0
#endif
#if defined(__ICL)
#define FMT_ICC_VERSION __ICL
#elif defined(__INTEL_COMPILER)
#define FMT_ICC_VERSION __INTEL_COMPILER
#else
#define FMT_ICC_VERSION 0
#endif
#if defined(_MSC_VER)
#define FMT_MSC_VERSION _MSC_VER
#else
#define FMT_MSC_VERSION 0
#endif
#ifdef _GLIBCXX_RELEASE
#define FMT_GLIBCXX_RELEASE _GLIBCXX_RELEASE
#else
#define FMT_GLIBCXX_RELEASE 0
#endif
#ifdef _LIBCPP_VERSION
#define FMT_LIBCPP_VERSION _LIBCPP_VERSION
#else
#define FMT_LIBCPP_VERSION 0
#endif
#ifdef _MSVC_LANG
#define FMT_CPLUSPLUS _MSVC_LANG
#else
#define FMT_CPLUSPLUS __cplusplus
#endif
#ifdef __has_feature
#define FMT_HAS_FEATURE(x) __has_feature(x)
#else
#define FMT_HAS_FEATURE(x) 0
#endif
#ifdef __has_include
#define FMT_HAS_INCLUDE(x) __has_include(x)
#else
#define FMT_HAS_INCLUDE(x) 0
#endif
#ifdef __has_cpp_attribute
#define FMT_HAS_CPP_ATTRIBUTE(x) __has_cpp_attribute(x)
#else
#define FMT_HAS_CPP_ATTRIBUTE(x) 0
#endif
#define FMT_HAS_CPP14_ATTRIBUTE(attribute) \
	(FMT_CPLUSPLUS >= 201402L && FMT_HAS_CPP_ATTRIBUTE(attribute))
#define FMT_HAS_CPP17_ATTRIBUTE(attribute) \
	(FMT_CPLUSPLUS >= 201703L && FMT_HAS_CPP_ATTRIBUTE(attribute))
#if defined(FMT_USE_NONTYPE_TEMPLATE_ARGS)
#elif defined(__NVCOMPILER)
#define FMT_USE_NONTYPE_TEMPLATE_ARGS 0
#elif FMT_GCC_VERSION >= 903 && FMT_CPLUSPLUS >= 201709L
#define FMT_USE_NONTYPE_TEMPLATE_ARGS 0
#elif defined(__cpp_nontype_template_args) && __cpp_nontype_template_args >= 201911L
#define FMT_USE_NONTYPE_TEMPLATE_ARGS 1
#else
#define FMT_USE_NONTYPE_TEMPLATE_ARGS 0
#endif
#ifdef FMT_EXCEPTIONS
#elif defined(__GNUC__) && !defined(__EXCEPTIONS)
#define FMT_EXCEPTIONS 0
#elif FMT_MSC_VERSION && !_HAS_EXCEPTIONS
#define FMT_EXCEPTIONS 0
#else
#define FMT_EXCEPTIONS 1
#endif
#if FMT_EXCEPTIONS
#define FMT_TRY try
#define FMT_CATCH(x) catch (x)
#else
#define FMT_TRY if (true)
#define FMT_CATCH(x) if (false)
#endif
#if FMT_HAS_CPP17_ATTRIBUTE(fallthrough)
#define FMT_FALLTHROUGH [[fallthrough]]
#elif defined(__clang__)
#define FMT_FALLTHROUGH [[clang::fallthrough]]
#elif FMT_GCC_VERSION >= 700 && (!defined(__EDG_VERSION__) || __EDG_VERSION__ >= 520)
#define FMT_FALLTHROUGH [[gnu::fallthrough]]
#else
#define FMT_FALLTHROUGH
#endif
#if FMT_HAS_CPP_ATTRIBUTE(noreturn) && FMT_EXCEPTIONS && !FMT_MSC_VERSION && !defined(__NVCC__)
#define FMT_NORETURN [[noreturn]]
#else
#define FMT_NORETURN
#endif
#ifndef FMT_NODISCARD
#if FMT_HAS_CPP17_ATTRIBUTE(nodiscard)
#define FMT_NODISCARD [[nodiscard]]
#else
#define FMT_NODISCARD
#endif
#endif
#ifdef FMT_DEPRECATED
#elif FMT_HAS_CPP14_ATTRIBUTE(deprecated)
#define FMT_DEPRECATED [[deprecated]]
#else
#define FMT_DEPRECATED
#endif
#ifdef FMT_INLINE
#elif FMT_GCC_VERSION || FMT_CLANG_VERSION
#define FMT_ALWAYS_INLINE inline __attribute__((always_inline))
#else
#define FMT_ALWAYS_INLINE inline
#endif
#ifdef NDEBUG
#define FMT_INLINE FMT_ALWAYS_INLINE
#else
#define FMT_INLINE inline
#endif
#if FMT_GCC_VERSION || FMT_CLANG_VERSION
#define FMT_VISIBILITY(value) __attribute__((visibility(value)))
#else
#define FMT_VISIBILITY(value)
#endif
#ifndef FMT_GCC_PRAGMA
#if FMT_GCC_VERSION >= 504 && !defined(__NVCOMPILER)
#define FMT_GCC_PRAGMA(arg) _Pragma(arg)
#else
#define FMT_GCC_PRAGMA(arg)
#endif
#endif
#if FMT_GCC_VERSION && FMT_GCC_VERSION < 500
#define FMT_DECLTYPE_THIS this->
#else
#define FMT_DECLTYPE_THIS
#endif
#if FMT_MSC_VERSION
#define FMT_MSC_WARNING(...) __pragma(warning(__VA_ARGS__))
#define FMT_UNCHECKED_ITERATOR(It) \
	using _Unchecked_type = It
#else
#define FMT_MSC_WARNING(...)
#define FMT_UNCHECKED_ITERATOR(It) using unchecked_type = It
#endif
#ifndef FMT_BEGIN_NAMESPACE
#define FMT_BEGIN_NAMESPACE \
	namespace fmt {         \
		inline namespace v10 {
#define FMT_END_NAMESPACE \
	}                     \
	}
#endif
#ifndef FMT_EXPORT
#define FMT_EXPORT
#define FMT_BEGIN_EXPORT
#define FMT_END_EXPORT
#endif
#if !defined(FMT_HEADER_ONLY) && defined(_WIN32)
#if defined(FMT_LIB_EXPORT)
#define FMT_API __declspec(dllexport)
#elif defined(FMT_SHARED)
#define FMT_API __declspec(dllimport)
#endif
#elif defined(FMT_LIB_EXPORT) || defined(FMT_SHARED)
#define FMT_API FMT_VISIBILITY("default")
#endif
#ifndef FMT_API
#define FMT_API
#endif
#ifndef FMT_UNICODE
#define FMT_UNICODE !FMT_MSC_VERSION
#endif
#define FMT_FWD(...) _sc<decltype(__VA_ARGS__)&&>(__VA_ARGS__)
FMT_GCC_PRAGMA("GCC push_options")
#if !defined(__OPTIMIZE__) && !defined(__CUDACC__)
FMT_GCC_PRAGMA("GCC optimize(\"Og\")")
#endif
FMT_BEGIN_NAMESPACE
_tp<bool B, _tn T = void> using enable_if_t = _tn std::enable_if<B, T>::type;
_tp<bool B, _tn T, _tn F> using conditional_t = _tn std::conditional<B, T, F>::type;
_tp<bool B> using bool_constant = std::integral_constant<bool, B>;
_tp<_tn T> using remove_reference_t = _tn std::remove_reference<T>::type;
_tp<_tn T> using remove_const_t = _tn std::remove_const<T>::type;
_tp<_tn T> using remove_cvref_t = _tn std::remove_cv<remove_reference_t<T>>::type;
_tp<_tn T> struct type_identity {
	using type = T;
};
_tp<_tn T> using type_identity_t = _tn type_identity<T>::type;
_tp<_tn T> using make_unsigned_t = _tn std::make_unsigned<T>::type;
_tp<_tn T> using underlying_t = _tn std::underlying_type<T>::type;
#if FMT_GCC_VERSION && FMT_GCC_VERSION < 500
_tp<_tn...> struct void_t_impl {
	using type = void;
};
_tp<_tn... T> using void_t = _tn void_t_impl<T...>::type;
#else
_tp<_tn...> using void_t = void;
#endif
struct monostate {
	_cx monostate() {}
};
#ifdef FMT_DOC
#define FMT_ENABLE_IF(...)
#else
#define FMT_ENABLE_IF(...) fmt::enable_if_t<(__VA_ARGS__), int> = 0
#endif
#ifdef __cpp_lib_byte
_tp<_tn T, FMT_ENABLE_IF(std::is_same<T, std::byte>::value)> inline auto format_as(T b) -> unsigned char {
	_rt _sc<unsigned char>(b);
}
#endif
namespace detail {
	_tp<_tn... T> _cx void ignore_unused(const T&...) {}
	_cx auto is_constant_evaluated(bool default_value = false) noexcept
		-> bool {
#if FMT_CPLUSPLUS >= 202002L && FMT_GLIBCXX_RELEASE >= 12 && (FMT_CLANG_VERSION >= 1400 && FMT_CLANG_VERSION < 1500)
		ignore_unused(default_value);
		_rt __builtin_is_constant_evaluated();
#elif defined(__cpp_lib_is_constant_evaluated)
		ignore_unused(default_value);
		_rt std::is_constant_evaluated();
#else
		_rt default_value;
#endif
	}
	_tp<_tn T> _cx auto const_check(T value) -> T {
		_rt value;
	}
	FMT_NORETURN FMT_API void assert_fail(const char* file, int line,
										  const char* message);
#ifndef FMT_ASSERT
#ifdef NDEBUG
#define FMT_ASSERT(condition, message) \
	fmt::detail::ignore_unused((condition), (message))
#else
#define FMT_ASSERT(condition, message) \
	((condition)                       \
		 ? (void)0                     \
		 : fmt::detail::assert_fail(__FILE__, __LINE__, (message)))
#endif
#endif
#ifdef FMT_USE_INT128
#elif defined(__SIZEOF_INT128__) && !defined(__NVCC__) && !(FMT_CLANG_VERSION && FMT_MSC_VERSION)
#define FMT_USE_INT128 1
	using int128_opt = __int128_t;
	using uint128_opt = __uint128_t;
	_tp<_tn T> inline auto convert_for_visit(T value) -> T {
		_rt value;
	}
#else
#define FMT_USE_INT128 0
#endif
#if !FMT_USE_INT128
	enum class int128_opt {};
	enum class uint128_opt {};
	_tp<_tn T> auto convert_for_visit(T) -> monostate {
		_rt{};
	}
#endif
	_tp<_tn Int> _cx auto to_unsigned(Int value) -> make_unsigned_t<Int> {
		FMT_ASSERT(std::is_unsigned<Int>::value || value >= 0, "negative value");
		_rt _sc<make_unsigned_t<Int>>(value);
	}
	_tp<_tn T, _tn Enable = void> struct is_std_string_like : std::false_type {};
	_tp<_tn T> struct is_std_string_like<T, void_t<decltype(std::declval<T>().find_first_of(
												_tn T::value_type(), 0))>>
		: std::true_type {};
	_cx inline auto is_utf8() -> bool {
		FMT_MSC_WARNING(suppress : 4566)
		_cx unsigned char section[] = "\u00A7";
		using uchar = unsigned char;
		_rt FMT_UNICODE || (sizeof(section) == 3 && uchar(section[0]) == 0xC2 && uchar(section[1]) == 0xA7);
	}
	_tp<_tn Char> _cx auto length(const Char* s) -> size_t {
		size_t len = 0;
		while (*s++) ++len;
		_rt len;
	}
	_tp<_tn Char> _cx auto compare(const Char* s1, const Char* s2, std::size_t n)
		-> int {
		for (; n != 0; ++s1, ++s2, --n) {
			if (*s1 < *s2) _rt - 1;
			if (*s1 > *s2) _rt 1;
		}
		_rt 0;
	}
	_tp<_tn It, _tn Enable = std::true_type> struct is_back_insert_iterator : std::false_type {};
	_tp<_tn It> struct is_back_insert_iterator<
		It,
		bool_constant<std::is_same<
			decltype(back_inserter(std::declval<_tn It::container_type&>())),
			It>::value>> : std::true_type {};
	_tp<_tn OutputIt> inline auto get_container(OutputIt it) -> _tn OutputIt::container_type& {
		struct accessor : OutputIt {
			accessor(OutputIt base)
				: OutputIt(base) {}
			using OutputIt::container;
		};
		_rt* accessor(it).container;
	}
} // namespace detail
_tp<_tn T> struct is_contiguous : std::false_type {};
FMT_EXPORT
_tp<_tn Char> class basic_string_view {
private:
	const Char* data_;
	size_t size_;

public:
	using value_type = Char;
	using iterator = const Char*;
	_cx basic_string_view() noexcept
		: data_(nullptr), size_(0) {}
	_cx basic_string_view(const Char* s, size_t count) noexcept
		: data_(s), size_(count) {}
	_cx basic_string_view(const Char* s)
		: data_(s),
		  size_(detail::const_check(std::is_same<Char, char>::value && !detail::is_constant_evaluated(false))
					? strlen(reinterpret_cast<const char*>(s))
					: detail::length(s)) {}
	_tp<_tn S,
		FMT_ENABLE_IF(detail::is_std_string_like<S>::value&& std::is_same<
					  _tn S::value_type, Char>::value)>
		_cx basic_string_view(const S& s) noexcept
		: data_(s.data()), size_(s.size()) {}
	_cx auto data() const noexcept -> const Char* { _rt data_; }
	_cx auto size() const noexcept -> size_t { _rt size_; }
	_cx auto begin() const noexcept -> iterator { _rt data_; }
	_cx auto end() const noexcept -> iterator { _rt data_ + size_; }
	_cx auto operator[](size_t pos) const noexcept -> const Char& {
		_rt data_[pos];
	}
	_cx void remove_prefix(size_t n) noexcept {
		data_ += n;
		size_ -= n;
	}
	_cx auto starts_with(basic_string_view<Char> sv) const noexcept
		-> bool {
		_rt size_ >= sv.size_&& detail::compare(data_, sv.data_, sv.size_) == 0;
	}
	_cx auto starts_with(Char c) const noexcept -> bool {
		_rt size_ >= 1 && *data_ == c;
	}
	_cx auto starts_with(const Char* s) const -> bool {
		_rt starts_with(basic_string_view<Char>(s));
	}
	_cx auto compare(basic_string_view other) const -> int {
		size_t str_size = size_ < other.size_ ? size_ : other.size_;
		int result = detail::compare(data_, other.data_, str_size);
		if (result == 0)
			result = size_ == other.size_ ? 0 : (size_ < other.size_ ? -1 : 1);
		_rt result;
	}
	_cx friend auto operator==(basic_string_view lhs,
							   basic_string_view rhs) -> bool {
		_rt lhs.compare(rhs) == 0;
	}
	friend auto operator!=(basic_string_view lhs, basic_string_view rhs) -> bool {
		_rt lhs.compare(rhs) != 0;
	}
	friend auto operator<(basic_string_view lhs, basic_string_view rhs) -> bool {
		_rt lhs.compare(rhs) < 0;
	}
	friend auto operator<=(basic_string_view lhs, basic_string_view rhs) -> bool {
		_rt lhs.compare(rhs) <= 0;
	}
	friend auto operator>(basic_string_view lhs, basic_string_view rhs) -> bool {
		_rt lhs.compare(rhs) > 0;
	}
	friend auto operator>=(basic_string_view lhs, basic_string_view rhs) -> bool {
		_rt lhs.compare(rhs) >= 0;
	}
};
FMT_EXPORT
using string_view = basic_string_view<char>;
FMT_EXPORT
_tp<_tn T> struct is_char : std::false_type {};
_tp<> struct is_char<char> : std::true_type {};
namespace detail {
	_tp<_tn Char, FMT_ENABLE_IF(is_char<Char>::value)> auto to_string_view(const Char* s) -> basic_string_view<Char> {
		_rt s;
	}
	_tp<_tn T, FMT_ENABLE_IF(is_std_string_like<T>::value)> auto to_string_view(const T& s) -> basic_string_view<_tn T::value_type> {
		_rt s;
	}
	_tp<_tn Char> _cx auto to_string_view(basic_string_view<Char> s)
		-> basic_string_view<Char> {
		_rt s;
	}
	_tp<_tn T, _tn Enable = void> struct has_to_string_view : std::false_type {};
	_tp<_tn T> struct has_to_string_view<
		T, void_t<decltype(detail::to_string_view(std::declval<T>()))>>
		: std::true_type {};
	_tp<_tn Char, Char... C> struct string_literal {
		static _cx Char value[sizeof...(C)] = { C... };
		_cx operator basic_string_view<Char>() const {
			_rt{ value, sizeof...(C) };
		}
	};
#if FMT_CPLUSPLUS < 201703L
	_tp<_tn Char, Char... C> _cx Char string_literal<Char, C...>::value[sizeof...(C)];
#endif
	enum class type {
		none_type,
		int_type,
		uint_type,
		long_long_type,
		ulong_long_type,
		int128_type,
		uint128_type,
		bool_type,
		char_type,
		last_integer_type = char_type,
		float_type,
		double_type,
		long_double_type,
		last_numeric_type = long_double_type,
		cstring_type,
		string_type,
		pointer_type,
		custom_type
	};
	_tp<_tn T, _tn Char> struct type_constant : std::integral_constant<type, type::custom_type> {};
#define FMT_TYPE_CONSTANT(Type, constant)          \
	_tp<_tn Char> struct type_constant<Type, Char> \
		: std::integral_constant<type, type::constant> {}
	FMT_TYPE_CONSTANT(int, int_type);
	FMT_TYPE_CONSTANT(unsigned, uint_type);
	FMT_TYPE_CONSTANT(ll, long_long_type);
	FMT_TYPE_CONSTANT(ull, ulong_long_type);
	FMT_TYPE_CONSTANT(int128_opt, int128_type);
	FMT_TYPE_CONSTANT(uint128_opt, uint128_type);
	FMT_TYPE_CONSTANT(bool, bool_type);
	FMT_TYPE_CONSTANT(Char, char_type);
	FMT_TYPE_CONSTANT(float, float_type);
	FMT_TYPE_CONSTANT(double, double_type);
	FMT_TYPE_CONSTANT(long double, long_double_type);
	FMT_TYPE_CONSTANT(const Char*, cstring_type);
	FMT_TYPE_CONSTANT(basic_string_view<Char>, string_type);
	FMT_TYPE_CONSTANT(const void*, pointer_type);
	_cx auto is_integral_type(type t) -> bool {
		_rt t > type::none_type&& t <= type::last_integer_type;
	}
	_cx auto is_arithmetic_type(type t) -> bool {
		_rt t > type::none_type&& t <= type::last_numeric_type;
	}
	_cx auto set(type rhs) -> int {
		_rt 1 << _sc<int>(rhs);
	}
	_cx auto in(type t, int set) -> bool {
		_rt((set >> _sc<int>(t)) & 1) != 0;
	}
	enum {
		sint_set =
			set(type::int_type) | set(type::long_long_type) | set(type::int128_type),
		uint_set = set(type::uint_type) | set(type::ulong_long_type) | set(type::uint128_type),
		bool_set = set(type::bool_type),
		char_set = set(type::char_type),
		float_set = set(type::float_type) | set(type::double_type) | set(type::long_double_type),
		string_set = set(type::string_type),
		cstring_set = set(type::cstring_type),
		pointer_set = set(type::pointer_type)
	};
} // namespace detail
FMT_NORETURN FMT_API void report_error(const char* message);
FMT_DEPRECATED FMT_NORETURN inline void throw_format_error(
	const char* message) {
	report_error(message);
}
_tp<_tn S,
	_tn V = decltype(detail::to_string_view(std::declval<S>()))> using char_t = _tn V::value_type;
FMT_EXPORT
_tp<_tn Char> class basic_format_parse_context {
private:
	basic_string_view<Char> format_str_;
	int next_arg_id_;
	_cx void do_check_arg_id(int id);

public:
	using char_type = Char;
	using iterator = const Char*;
	explicit _cx basic_format_parse_context(
		basic_string_view<Char> format_str, int next_arg_id = 0)
		: format_str_(format_str), next_arg_id_(next_arg_id) {}
	_cx auto begin() const noexcept -> iterator {
		_rt format_str_.begin();
	}
	_cx auto end() const noexcept -> iterator { _rt format_str_.end(); }
	_cx void advance_to(iterator it) {
		format_str_.remove_prefix(detail::to_unsigned(it - begin()));
	}
	_cx auto next_arg_id() -> int {
		if (next_arg_id_ < 0) {
			report_error("cannot switch from manual to automatic argument indexing");
			_rt 0;
		}
		int id = next_arg_id_++;
		do_check_arg_id(id);
		_rt id;
	}
	_cx void check_arg_id(int id) {
		if (next_arg_id_ > 0) {
			report_error("cannot switch from automatic to manual argument indexing");
			_rt;
		}
		next_arg_id_ = -1;
		do_check_arg_id(id);
	}
	_cx void check_arg_id(basic_string_view<Char>) {
		next_arg_id_ = -1;
	}
	_cx void check_dynamic_spec(int arg_id);
};
FMT_EXPORT
using format_parse_context = basic_format_parse_context<char>;
namespace detail {
	_tp<_tn Char> class compile_parse_context : public basic_format_parse_context<Char> {
	private:
		int num_args_;
		const type* types_;
		using base = basic_format_parse_context<Char>;

	public:
		explicit _cx compile_parse_context(
			basic_string_view<Char> format_str, int num_args, const type* types,
			int next_arg_id = 0)
			: base(format_str, next_arg_id), num_args_(num_args), types_(types) {}
		_cx auto num_args() const -> int { _rt num_args_; }
		_cx auto arg_type(int id) const -> type { _rt types_[id]; }
		_cx auto next_arg_id() -> int {
			int id = base::next_arg_id();
			if (id >= num_args_) report_error("argument not found");
			_rt id;
		}
		_cx void check_arg_id(int id) {
			base::check_arg_id(id);
			if (id >= num_args_) report_error("argument not found");
		}
		using base::check_arg_id;
		_cx void check_dynamic_spec(int arg_id) {
			detail::ignore_unused(arg_id);
			if (arg_id < num_args_ && types_ && !is_integral_type(types_[arg_id]))
				report_error("width/precision is not integer");
		}
	};
	_tp<_tn T> class buffer {
	private:
		T* ptr_;
		size_t size_;
		size_t capacity_;
		using grow_fun = void (*)(buffer& buf, size_t capacity);
		grow_fun grow_;

	protected:
		FMT_MSC_WARNING(suppress : 26495)
		_cx buffer(grow_fun grow, size_t sz) noexcept
			: size_(sz), capacity_(sz), grow_(grow) {}
		_cx buffer(grow_fun grow, T* p = nullptr, size_t sz = 0,
				   size_t cap = 0) noexcept
			: ptr_(p), size_(sz), capacity_(cap), grow_(grow) {}
		_cx ~buffer() = default;
		buffer(buffer&&) = default;
		_cx void set(T* buf_data, size_t buf_capacity) noexcept {
			ptr_ = buf_data;
			capacity_ = buf_capacity;
		}

	public:
		using value_type = T;
		using const_reference = const T&;
		buffer(const buffer&) = delete;
		void operator=(const buffer&) = delete;
		auto begin() noexcept -> T* { _rt ptr_; }
		auto end() noexcept -> T* { _rt ptr_ + size_; }
		auto begin() const noexcept -> const T* { _rt ptr_; }
		auto end() const noexcept -> const T* { _rt ptr_ + size_; }
		_cx auto size() const noexcept -> size_t { _rt size_; }
		_cx auto capacity() const noexcept -> size_t { _rt capacity_; }
		_cx auto data() noexcept -> T* { _rt ptr_; }
		_cx auto data() const noexcept -> const T* { _rt ptr_; }
		void clear() { size_ = 0; }
		_cx void try_resize(size_t count) {
			try_reserve(count);
			size_ = count <= capacity_ ? count : capacity_;
		}
		_cx void try_reserve(size_t new_capacity) {
			if (new_capacity > capacity_) grow_(*this, new_capacity);
		}
		_cx void push_back(const T& value) {
			try_reserve(size_ + 1);
			ptr_[size_++] = value;
		}
		_tp<_tn U> void append(const U* begin, const U* end) {
			while (begin != end) {
				auto count = to_unsigned(end - begin);
				try_reserve(size_ + count);
				auto free_cap = capacity_ - size_;
				if (free_cap < count) count = free_cap;
				if (std::is_same<T, U>::value) {
					memcpy(ptr_ + size_, begin, count * sizeof(T));
				}
				else {
					T* out = ptr_ + size_;
					for (size_t i = 0; i < count; ++i) out[i] = begin[i];
				}
				size_ += count;
				begin += count;
			}
		}
		_tp<_tn Idx> _cx auto operator[](Idx index) -> T& {
			_rt ptr_[index];
		}
		_tp<_tn Idx> _cx auto operator[](Idx index) const -> const T& {
			_rt ptr_[index];
		}
	};
	struct buffer_traits {
		explicit buffer_traits(size_t) {}
		auto count() const -> size_t { _rt 0; }
		auto limit(size_t size) -> size_t { _rt size; }
	};
	class fixed_buffer_traits {
	private:
		size_t count_ = 0;
		size_t limit_;

	public:
		explicit fixed_buffer_traits(size_t limit)
			: limit_(limit) {}
		auto count() const -> size_t { _rt count_; }
		auto limit(size_t size) -> size_t {
			size_t n = limit_ > count_ ? limit_ - count_ : 0;
			count_ += size;
			_rt size < n ? size : n;
		}
	};
	_tp<_tn OutputIt, _tn T, _tn Traits = buffer_traits> class iterator_buffer : public Traits, public buffer<T> {
	private:
		OutputIt out_;
		enum { buffer_size = 256 };
		T data_[buffer_size];
		static _cx void grow(buffer<T>& buf, size_t) {
			if (buf.size() == buffer_size) _sc<iterator_buffer&>(buf).flush();
		}
		void flush() {
			auto size = this->size();
			this->clear();
			const T* begin = data_;
			const T* end = begin + this->limit(size);
			while (begin != end) *out_++ = *begin++;
		}

	public:
		explicit iterator_buffer(OutputIt out, size_t n = buffer_size)
			: Traits(n), buffer<T>(grow, data_, 0, buffer_size), out_(out) {}
		iterator_buffer(iterator_buffer&& other) noexcept
			: Traits(other),
			  buffer<T>(grow, data_, 0, buffer_size),
			  out_(other.out_) {}
		~iterator_buffer() {
			FMT_TRY {
				flush();
			}
			FMT_CATCH(...) {}
		}
		auto out() -> OutputIt {
			flush();
			_rt out_;
		}
		auto count() const -> size_t { _rt Traits::count() + this->size(); }
	};
	_tp<_tn T> class iterator_buffer<T*, T, fixed_buffer_traits> : public fixed_buffer_traits,
																   public buffer<T> {
	private:
		T* out_;
		enum { buffer_size = 256 };
		T data_[buffer_size];
		static _cx void grow(buffer<T>& buf, size_t) {
			if (buf.size() == buf.capacity())
				_sc<iterator_buffer&>(buf).flush();
		}
		void flush() {
			size_t n = this->limit(this->size());
			if (this->data() == out_) {
				out_ += n;
				this->set(data_, buffer_size);
			}
			this->clear();
		}

	public:
		explicit iterator_buffer(T* out, size_t n = buffer_size)
			: fixed_buffer_traits(n), buffer<T>(grow, out, 0, n), out_(out) {}
		iterator_buffer(iterator_buffer&& other) noexcept
			: fixed_buffer_traits(other),
			  buffer<T>(_sc<iterator_buffer&&>(other)),
			  out_(other.out_) {
			if (this->data() != out_) {
				this->set(data_, buffer_size);
				this->clear();
			}
		}
		~iterator_buffer() { flush(); }
		auto out() -> T* {
			flush();
			_rt out_;
		}
		auto count() const -> size_t {
			_rt fixed_buffer_traits::count() + this->size();
		}
	};
	_tp<_tn T> class iterator_buffer<T*, T> : public buffer<T> {
	public:
		explicit iterator_buffer(T* out, size_t = 0)
			: buffer<T>([](buffer<T>&, size_t) {}, out, 0, ~size_t()) {}
		auto out() -> T* { _rt&* this->end(); }
	};
	_tp<_tn OutputIt> class iterator_buffer<
		OutputIt,
		enable_if_t<detail::is_back_insert_iterator<OutputIt>::value && is_contiguous<_tn OutputIt::container_type>::value,
					_tn OutputIt::container_type::value_type>>
		: public buffer<_tn OutputIt::container_type::value_type> {
	private:
		using container_type = _tn OutputIt::container_type;
		using value_type = _tn container_type::value_type;
		container_type& container_;
		static _cx void grow(buffer<value_type>& buf, size_t capacity) {
			auto& self = _sc<iterator_buffer&>(buf);
			self.container_.resize(capacity);
			self.set(&self.container_[0], capacity);
		}

	public:
		explicit iterator_buffer(container_type& c)
			: buffer<value_type>(grow, c.size()), container_(c) {}
		explicit iterator_buffer(OutputIt out, size_t = 0)
			: iterator_buffer(get_container(out)) {}
		auto out() -> OutputIt { _rt back_inserter(container_); }
	};
	_tp<_tn T = char> class counting_buffer : public buffer<T> {
	private:
		enum { buffer_size = 256 };
		T data_[buffer_size];
		size_t count_ = 0;
		static _cx void grow(buffer<T>& buf, size_t) {
			if (buf.size() != buffer_size) _rt;
			_sc<counting_buffer&>(buf).count_ += buf.size();
			buf.clear();
		}

	public:
		counting_buffer()
			: buffer<T>(grow, data_, 0, buffer_size) {}
		auto count() -> size_t { _rt count_ + this->size(); }
	};
} // namespace detail
_tp<_tn Char> _cx void basic_format_parse_context<Char>::do_check_arg_id(int id) {
	if (detail::is_constant_evaluated() && (!FMT_GCC_VERSION || FMT_GCC_VERSION >= 1200)) {
		using context = detail::compile_parse_context<Char>;
		if (id >= _sc<context*>(this)->num_args())
			report_error("argument not found");
	}
}
_tp<_tn Char> _cx void basic_format_parse_context<Char>::check_dynamic_spec(
	int arg_id) {
	if (detail::is_constant_evaluated() && (!FMT_GCC_VERSION || FMT_GCC_VERSION >= 1200)) {
		using context = detail::compile_parse_context<Char>;
		_sc<context*>(this)->check_dynamic_spec(arg_id);
	}
}
FMT_EXPORT _tp<_tn Context> class basic_format_arg;
FMT_EXPORT _tp<_tn Context> class basic_format_args;
FMT_EXPORT _tp<_tn Context> class dynamic_format_arg_store;
FMT_EXPORT
_tp<_tn T, _tn Char = char, _tn Enable = void> struct formatter {
	formatter() = delete;
};
_tp<_tn T, _tn Context> using has_formatter =
	std::is_constructible<_tn Context::_tp formatter_type<T>>;
_tp<_tn T> class basic_appender {
private:
	detail::buffer<T>* buffer_;
	friend auto get_container(basic_appender app) -> detail::buffer<T>& {
		_rt* app.buffer_;
	}

public:
	using iterator_category = int;
	using value_type = T;
	using difference_type = ptrdiff_t;
	using pointer = T*;
	using reference = T&;
	FMT_UNCHECKED_ITERATOR(basic_appender);
	_cx basic_appender(detail::buffer<T>& buf)
		: buffer_(&buf) {}
	auto operator=(T c) -> basic_appender& {
		buffer_->push_back(c);
		_rt* this;
	}
	auto operator*() -> basic_appender& { _rt* this; }
	auto operator++() -> basic_appender& { _rt* this; }
	auto operator++(int) -> basic_appender { _rt* this; }
};
using appender = basic_appender<char>;
namespace detail {
	_tp<_tn T, _tn InputIt> auto copy(InputIt begin, InputIt end, appender out) -> appender {
		get_container(out).append(begin, end);
		_rt out;
	}
	_tp<_tn T, _tn InputIt, _tn OutputIt,
		FMT_ENABLE_IF(is_back_insert_iterator<OutputIt>::value)> auto
	copy(InputIt begin, InputIt end, OutputIt out) -> OutputIt {
		get_container(out).append(begin, end);
		_rt out;
	}
	_tp<_tn T, _tn InputIt, _tn OutputIt,
		FMT_ENABLE_IF(!is_back_insert_iterator<OutputIt>::value)>
		_cx auto
		copy(InputIt begin, InputIt end, OutputIt out) -> OutputIt {
		while (begin != end) *out++ = _sc<T>(*begin++);
		_rt out;
	}
	_tp<_tn T> _cx auto copy(const T* begin, const T* end, T* out) -> T* {
		if (is_constant_evaluated()) _rt copy<T, const T*, T*>(begin, end, out);
		auto size = to_unsigned(end - begin);
		if (size > 0) memcpy(out, begin, size * sizeof(T));
		_rt out + size;
	}
	_tp<_tn T, _tn V, _tn OutputIt> _cx auto copy(basic_string_view<V> s, OutputIt out) -> OutputIt {
		_rt copy<T>(s.begin(), s.end(), out);
	}
	_tp<_tn Context, _tn T> _cx auto has_const_formatter_impl(T*)
		-> decltype(_tn Context::_tp formatter_type<T>().format(
						std::declval<const T&>(), std::declval<Context&>()),
					true) {
		_rt true;
	}
	_tp<_tn Context> _cx auto has_const_formatter_impl(...) -> bool {
		_rt false;
	}
	_tp<_tn T, _tn Context> _cx auto has_const_formatter() -> bool {
		_rt has_const_formatter_impl<Context>(_sc<T*>(nullptr));
	}
	_tp<_tn T, _tn OutputIt> auto get_buffer(OutputIt out) -> iterator_buffer<OutputIt, T> {
		_rt iterator_buffer<OutputIt, T>(out);
	}
	_tp<_tn T> auto get_buffer(basic_appender<T> out) -> buffer<T>& {
		_rt get_container(out);
	}
	_tp<_tn Buf, _tn OutputIt> auto get_iterator(Buf& buf, OutputIt) -> decltype(buf.out()) {
		_rt buf.out();
	}
	_tp<_tn T, _tn OutputIt> auto get_iterator(buffer<T>&, OutputIt out) -> OutputIt {
		_rt out;
	}
	struct view {};
	_tp<_tn Char, _tn T> struct named_arg : view {
		const Char* name;
		const T& value;
		named_arg(const Char* n, const T& v)
			: name(n), value(v) {}
	};
	_tp<_tn Char> struct named_arg_info {
		const Char* name;
		int id;
	};
	_tp<_tn T> struct is_named_arg : std::false_type {};
	_tp<_tn T> struct is_statically_named_arg : std::false_type {};
	_tp<_tn T, _tn Char> struct is_named_arg<named_arg<Char, T>> : std::true_type {};
	_tp<bool B = false> _cx auto count() -> size_t {
		_rt B ? 1 : 0;
	}
	_tp<bool B1, bool B2, bool... Tail> _cx auto count() -> size_t {
		_rt(B1 ? 1 : 0) + count<B2, Tail...>();
	}
	_tp<_tn... Args> _cx auto count_named_args() -> size_t {
		_rt count<is_named_arg<Args>::value...>();
	}
	_tp<_tn... Args> _cx auto count_statically_named_args() -> size_t {
		_rt count<is_statically_named_arg<Args>::value...>();
	}
	struct unformattable {};
	struct unformattable_char : unformattable {};
	struct unformattable_pointer : unformattable {};
	_tp<_tn Char> struct string_value {
		const Char* data;
		size_t size;
	};
	_tp<_tn Char> struct named_arg_value {
		const named_arg_info<Char>* data;
		size_t size;
	};
	_tp<_tn Context> struct custom_value {
		using parse_context = _tn Context::parse_context_type;
		void* value;
		void (*format)(void* arg, parse_context& parse_ctx, Context& ctx);
	};
	_tp<_tn Context> class value {
	public:
		using char_type = _tn Context::char_type;
		union {
			monostate no_value;
			int int_value;
			unsigned uint_value;
			ll long_long_value;
			ull ulong_long_value;
			int128_opt int128_value;
			uint128_opt uint128_value;
			bool bool_value;
			char_type char_value;
			float float_value;
			double double_value;
			long double long_double_value;
			const void* pointer;
			string_value<char_type> string;
			custom_value<Context> custom;
			named_arg_value<char_type> named_args;
		};
		_cx FMT_ALWAYS_INLINE value()
			: no_value() {}
		_cx FMT_ALWAYS_INLINE value(int val)
			: int_value(val) {}
		_cx FMT_ALWAYS_INLINE value(unsigned val)
			: uint_value(val) {}
		_cx FMT_ALWAYS_INLINE value(ll val)
			: long_long_value(val) {}
		_cx FMT_ALWAYS_INLINE value(ull val)
			: ulong_long_value(val) {}
		FMT_ALWAYS_INLINE value(int128_opt val)
			: int128_value(val) {}
		FMT_ALWAYS_INLINE value(uint128_opt val)
			: uint128_value(val) {}
		_cx FMT_ALWAYS_INLINE value(float val)
			: float_value(val) {}
		_cx FMT_ALWAYS_INLINE value(double val)
			: double_value(val) {}
		FMT_ALWAYS_INLINE value(long double val)
			: long_double_value(val) {}
		_cx FMT_ALWAYS_INLINE value(bool val)
			: bool_value(val) {}
		_cx FMT_ALWAYS_INLINE value(char_type val)
			: char_value(val) {}
		_cx FMT_ALWAYS_INLINE value(const char_type* val) {
			string.data = val;
			if (is_constant_evaluated()) string.size = {};
		}
		_cx FMT_ALWAYS_INLINE value(basic_string_view<char_type> val) {
			string.data = val.data();
			string.size = val.size();
		}
		FMT_ALWAYS_INLINE value(const void* val)
			: pointer(val) {}
		FMT_ALWAYS_INLINE value(const named_arg_info<char_type>* args, size_t size)
			: named_args{ args, size } {}
		_tp<_tn T> _cx FMT_ALWAYS_INLINE value(T& val) {
			using value_type = remove_const_t<T>;
#if defined(__cpp_if__cx)
			if _cx (std::is_same<decltype(&val), T*>::value)
				custom.value = const_cast<value_type*>(&val);
#endif
			if (!is_constant_evaluated())
				custom.value = const_cast<char*>(&reinterpret_cast<const char&>(val));
			custom.format = format_custom_arg<
				value_type, _tn Context::_tp formatter_type<value_type>>;
		}
		value(unformattable);
		value(unformattable_char);
		value(unformattable_pointer);

	private:
		_tp<_tn T, _tn Formatter> static void format_custom_arg(void* arg,
																_tn Context::parse_context_type& parse_ctx,
																Context& ctx) {
			auto f = Formatter();
			parse_ctx.advance_to(f.parse(parse_ctx));
			using qualified_type =
				conditional_t<has_const_formatter<T, Context>(), const T, T>;
			const auto& cf = f;
			ctx.advance_to(cf.format(*_sc<qualified_type*>(arg), ctx));
		}
	};
	enum { long_short = sizeof(long) == sizeof(int) };
	using long_type = conditional_t<long_short, int, ll>;
	using ulong_type = conditional_t<long_short, unsigned, ull>;
	_tp<_tn T> struct format_as_result {
		_tp<_tn U,
			FMT_ENABLE_IF(std::is_enum<U>::value || std::is_class<U>::value)> static auto
		map(U*) -> remove_cvref_t<decltype(format_as(std::declval<U>()))>;
		static auto map(...) -> void;
		using type = decltype(map(_sc<T*>(nullptr)));
	};
	_tp<_tn T> using format_as_t = _tn format_as_result<T>::type;
	_tp<_tn T> struct has_format_as
		: bool_constant<!std::is_same<format_as_t<T>, void>::value> {};
#define FMT_MAP_API _cx FMT_ALWAYS_INLINE
	_tp<_tn Context> struct arg_mapper {
		using char_type = _tn Context::char_type;
		FMT_MAP_API auto map(signed char val) -> int { _rt val; }
		FMT_MAP_API auto map(unsigned char val) -> unsigned { _rt val; }
		FMT_MAP_API auto map(short val) -> int { _rt val; }
		FMT_MAP_API auto map(unsigned short val) -> unsigned { _rt val; }
		FMT_MAP_API auto map(int val) -> int { _rt val; }
		FMT_MAP_API auto map(unsigned val) -> unsigned { _rt val; }
		FMT_MAP_API auto map(long val) -> long_type { _rt val; }
		FMT_MAP_API auto map(unsigned long val) -> ulong_type { _rt val; }
		FMT_MAP_API auto map(ll val) -> ll { _rt val; }
		FMT_MAP_API auto map(ull val) -> ull {
			_rt val;
		}
		FMT_MAP_API auto map(int128_opt val) -> int128_opt { _rt val; }
		FMT_MAP_API auto map(uint128_opt val) -> uint128_opt { _rt val; }
		FMT_MAP_API auto map(bool val) -> bool { _rt val; }
		_tp<_tn T, FMT_ENABLE_IF(std::is_same<T, char>::value || std::is_same<T, char_type>::value)>
			FMT_MAP_API auto map(T val) -> char_type {
			_rt val;
		}
		_tp<_tn T, enable_if_t<(std::is_same<T, wchar_t>::value ||
#ifdef __cpp_char8_t
								std::is_same<T, char8_t>::value ||
#endif
								std::is_same<T, char16_t>::value || std::is_same<T, char32_t>::value)
								   && !std::is_same<T, char_type>::value,
							   int> = 0>
			FMT_MAP_API auto map(T) -> unformattable_char {
			_rt{};
		}
		FMT_MAP_API auto map(float val) -> float { _rt val; }
		FMT_MAP_API auto map(double val) -> double { _rt val; }
		FMT_MAP_API auto map(long double val) -> long double { _rt val; }
		FMT_MAP_API auto map(char_type* val) -> const char_type* { _rt val; }
		FMT_MAP_API auto map(const char_type* val) -> const char_type* { _rt val; }
		_tp<_tn T, _tn Char = char_t<T>,
			FMT_ENABLE_IF(std::is_same<Char, char_type>::value && !std::is_pointer<T>::value)>
			FMT_MAP_API auto map(const T& val) -> basic_string_view<Char> {
			_rt to_string_view(val);
		}
		_tp<_tn T, _tn Char = char_t<T>,
			FMT_ENABLE_IF(!std::is_same<Char, char_type>::value && !std::is_pointer<T>::value)>
			FMT_MAP_API auto map(const T&) -> unformattable_char {
			_rt{};
		}
		FMT_MAP_API auto map(void* val) -> const void* { _rt val; }
		FMT_MAP_API auto map(const void* val) -> const void* { _rt val; }
		FMT_MAP_API auto map(std::nullptr_t val) -> const void* { _rt val; }
		_tp<
			_tn T,
			FMT_ENABLE_IF(
				std::is_pointer<T>::value || std::is_member_pointer<T>::value || std::is_function<_tn std::remove_pointer<T>::type>::value || (std::is_array<T>::value && !std::is_convertible<T, const char_type*>::value))>
			_cx auto
			map(const T&) -> unformattable_pointer {
			_rt{};
		}
		_tp<_tn T, std::size_t N,
			FMT_ENABLE_IF(!std::is_same<T, wchar_t>::value)>
			FMT_MAP_API auto map(const T (&values)[N]) -> const T (&)[N] {
			_rt values;
		}
		_tp<_tn T, _tn U = format_as_t<T>,
			FMT_ENABLE_IF(std::is_arithmetic<U>::value)>
			FMT_MAP_API auto map(const T& val) -> decltype(FMT_DECLTYPE_THIS map(U())) {
			_rt map(format_as(val));
		}
		_tp<_tn T, _tn U = remove_const_t<T>> struct formattable : bool_constant<has_const_formatter<U, Context>() || (has_formatter<U, Context>::value && !std::is_const<T>::value)> {};
		_tp<_tn T, FMT_ENABLE_IF(formattable<T>::value)>
			FMT_MAP_API auto do_map(T& val) -> T& {
			_rt val;
		}
		_tp<_tn T, FMT_ENABLE_IF(!formattable<T>::value)>
			FMT_MAP_API auto do_map(T&) -> unformattable {
			_rt{};
		}
		_tp<_tn T, _tn U = remove_const_t<T>,
			FMT_ENABLE_IF((std::is_class<U>::value || std::is_enum<U>::value || std::is_union<U>::value) && !has_to_string_view<U>::value && !is_char<U>::value && !is_named_arg<U>::value && !std::is_arithmetic<format_as_t<U>>::value)>
			FMT_MAP_API auto map(T& val) -> decltype(FMT_DECLTYPE_THIS do_map(val)) {
			_rt do_map(val);
		}
		_tp<_tn T, FMT_ENABLE_IF(is_named_arg<T>::value)>
			FMT_MAP_API auto map(const T& named_arg)
				-> decltype(FMT_DECLTYPE_THIS map(named_arg.value)) {
			_rt map(named_arg.value);
		}
		auto map(...) -> unformattable { _rt{}; }
	};
	_tp<_tn T, _tn Context> using mapped_type_constant =
		type_constant<decltype(arg_mapper<Context>().map(std::declval<const T&>())),
					  _tn Context::char_type>;
	enum { packed_arg_bits = 4 };
	enum { max_packed_args = 62 / packed_arg_bits };
	enum : ull { is_unpacked_bit = 1ULL << 63 };
	enum : ull { has_named_args_bit = 1ULL << 62 };
	_tp<_tn It, _tn T, _tn Enable = void> struct is_output_iterator : std::false_type {};
	_tp<> struct is_output_iterator<appender, char> : std::true_type {};
	_tp<_tn It, _tn T> struct is_output_iterator<
		It, T, void_t<decltype(*std::declval<It&>()++ = std::declval<T>())>>
		: std::true_type {};
	class locale_ref {
	private:
		const void* locale_;

	public:
		_cx locale_ref()
			: locale_(nullptr) {}
		_tp<_tn Locale> explicit locale_ref(const Locale& loc);
		explicit operator bool() const noexcept { _rt locale_ != nullptr; }
		_tp<_tn Locale> auto get() const -> Locale;
	};
	_tp<_tn> _cx auto encode_types() -> ull {
		_rt 0;
	}
	_tp<_tn Context, _tn Arg, _tn... Args> _cx auto encode_types() -> ull {
		_rt _sc<unsigned>(mapped_type_constant<Arg, Context>::value) | (encode_types<Context, Args...>() << packed_arg_bits);
	}
	_tp<_tn Context, _tn... T, size_t NUM_ARGS = sizeof...(T)> _cx ull make_descriptor() {
		_rt NUM_ARGS <= max_packed_args ? encode_types<Context, T...>()
										: is_unpacked_bit | NUM_ARGS;
	}
	_tp<_tn T, _tn Char>
#if FMT_CLANG_VERSION && FMT_CLANG_VERSION <= 1500
		struct type_is_unformattable_for {
	};
#else
		struct type_is_unformattable_for;
#endif
	_tp<bool PACKED, _tn Context, _tn T, FMT_ENABLE_IF(PACKED)> _cx auto make_arg(T& val) -> value<Context> {
		using arg_type = remove_cvref_t<decltype(arg_mapper<Context>().map(val))>;
		enum {
			formattable_char = !std::is_same<arg_type, unformattable_char>::value
		};
		static_assert(formattable_char, "Mixing character types is disallowed.");
		enum {
			formattable_pointer = !std::is_same<arg_type, unformattable_pointer>::value
		};
		static_assert(formattable_pointer,
					  "Formatting of non-void pointers is disallowed.");
		enum { formattable = !std::is_same<arg_type, unformattable>::value };
#if defined(__cpp_if__cx)
		if _cx (!formattable)
			type_is_unformattable_for<T, _tn Context::char_type> _;
#endif
		static_assert(
			formattable,
			"Cannot format an argument.");
		_rt{ arg_mapper<Context>().map(val) };
	}
	_tp<_tn Context, _tn T> _cx auto make_arg(T& val) -> basic_format_arg<Context> {
		auto arg = basic_format_arg<Context>();
		arg.type_ = mapped_type_constant<T, Context>::value;
		arg.value_ = make_arg<true, Context>(val);
		_rt arg;
	}
	_tp<bool PACKED, _tn Context, _tn T, FMT_ENABLE_IF(!PACKED)> _cx inline auto make_arg(T& val) -> basic_format_arg<Context> {
		_rt make_arg<Context>(val);
	}
	_tp<_tn Context, size_t NUM_ARGS> using arg_t = conditional_t<NUM_ARGS <= max_packed_args, value<Context>,
																  basic_format_arg<Context>>;
	_tp<_tn Char, _tn T, FMT_ENABLE_IF(!is_named_arg<T>::value)> void init_named_arg(named_arg_info<Char>*, int& arg_index, int&, const T&) {
		++arg_index;
	}
	_tp<_tn Char, _tn T, FMT_ENABLE_IF(is_named_arg<T>::value)> void init_named_arg(named_arg_info<Char>* named_args, int& arg_index,
																					int& named_arg_index, const T& arg) {
		named_args[named_arg_index++] = { arg.name, arg_index++ };
	}
	_tp<_tn Context, size_t NUM_ARGS, size_t NUM_NAMED_ARGS,
		ull DESC> struct format_arg_store {
		static _cx size_t ARGS_ARR_SIZE = 1 + (NUM_ARGS != 0 ? NUM_ARGS : +1);
		arg_t<Context, NUM_ARGS> args[ARGS_ARR_SIZE];
		named_arg_info<_tn Context::char_type> named_args[NUM_NAMED_ARGS];
		_tp<_tn... T>
			FMT_MAP_API format_arg_store(T&... values)
			: args{ { named_args, NUM_NAMED_ARGS },
					make_arg<NUM_ARGS <= max_packed_args, Context>(values)... } {
			using dummy = int[];
			int arg_index = 0, named_arg_index = 0;
			(void)dummy{
				0,
				(init_named_arg(named_args, arg_index, named_arg_index, values), 0)...
			};
		}
		format_arg_store(format_arg_store&& rhs) {
			args[0] = { named_args, NUM_NAMED_ARGS };
			for (size_t i = 1; i < ARGS_ARR_SIZE; ++i) args[i] = rhs.args[i];
			for (size_t i = 0; i < NUM_NAMED_ARGS; ++i)
				named_args[i] = rhs.named_args[i];
		}
		format_arg_store(const format_arg_store& rhs) = delete;
		format_arg_store& operator=(const format_arg_store& rhs) = delete;
		format_arg_store& operator=(format_arg_store&& rhs) = delete;
	};
	_tp<_tn Context, size_t NUM_ARGS, ull DESC> struct format_arg_store<Context, NUM_ARGS, 0, DESC> {
		arg_t<Context, NUM_ARGS> args[NUM_ARGS != 0 ? NUM_ARGS : +1];
	};
} // namespace detail
FMT_BEGIN_EXPORT
_tp<_tn Context> class basic_format_arg {
private:
	detail::value<Context> value_;
	detail::type type_;
	_tp<_tn ContextType, _tn T> friend _cx auto detail::make_arg(T& value)
		-> basic_format_arg<ContextType>;
	friend class basic_format_args<Context>;
	friend class dynamic_format_arg_store<Context>;
	using char_type = _tn Context::char_type;
	_tp<_tn, size_t, size_t, ull> friend struct detail::format_arg_store;
	basic_format_arg(const detail::named_arg_info<char_type>* args, size_t size)
		: value_(args, size) {}

public:
	class handle {
	public:
		explicit handle(detail::custom_value<Context> custom)
			: custom_(custom) {}
		void format(_tn Context::parse_context_type& parse_ctx,
					Context& ctx) const {
			custom_.format(custom_.value, parse_ctx, ctx);
		}

	private:
		detail::custom_value<Context> custom_;
	};
	_cx basic_format_arg()
		: type_(detail::type::none_type) {}
	_cx explicit operator bool() const noexcept {
		_rt type_ != detail::type::none_type;
	}
	auto type() const -> detail::type { _rt type_; }
	auto is_integral() const -> bool { _rt detail::is_integral_type(type_); }
	auto is_arithmetic() const -> bool {
		_rt detail::is_arithmetic_type(type_);
	}
	_tp<_tn Visitor> _cx auto visit(Visitor&& vis) -> decltype(vis(0)) {
		switch (type_) {
		case detail::type::none_type:
			break;
		case detail::type::int_type:
			_rt vis(value_.int_value);
		case detail::type::uint_type:
			_rt vis(value_.uint_value);
		case detail::type::long_long_type:
			_rt vis(value_.long_long_value);
		case detail::type::ulong_long_type:
			_rt vis(value_.ulong_long_value);
		case detail::type::int128_type:
			_rt vis(detail::convert_for_visit(value_.int128_value));
		case detail::type::uint128_type:
			_rt vis(detail::convert_for_visit(value_.uint128_value));
		case detail::type::bool_type:
			_rt vis(value_.bool_value);
		case detail::type::char_type:
			_rt vis(value_.char_value);
		case detail::type::float_type:
			_rt vis(value_.float_value);
		case detail::type::double_type:
			_rt vis(value_.double_value);
		case detail::type::long_double_type:
			_rt vis(value_.long_double_value);
		case detail::type::cstring_type:
			_rt vis(value_.string.data);
		case detail::type::string_type:
			using sv = basic_string_view<_tn Context::char_type>;
			_rt vis(sv(value_.string.data, value_.string.size));
		case detail::type::pointer_type:
			_rt vis(value_.pointer);
		case detail::type::custom_type:
			_rt vis(_tn basic_format_arg<Context>::handle(value_.custom));
		}
		_rt vis(monostate());
	}
	auto format_custom(const char_type* parse_begin,
					   _tn Context::parse_context_type& parse_ctx,
					   Context& ctx) -> bool {
		if (type_ != detail::type::custom_type) _rt false;
		parse_ctx.advance_to(parse_begin);
		value_.custom.format(value_.custom.value, parse_ctx, ctx);
		_rt true;
	}
};
_tp<_tn Visitor, _tn Context>
	FMT_DEPRECATED _cx auto visit_format_arg(
		Visitor&& vis, const basic_format_arg<Context>& arg) -> decltype(vis(0)) {
	_rt arg.visit(_sc<Visitor&&>(vis));
}
_tp<_tn Context> class basic_format_args {
public:
	using size_type = int;
	using format_arg = basic_format_arg<Context>;

private:
	ull desc_;
	union {
		const detail::value<Context>* values_;
		const format_arg* args_;
	};
	_cx auto is_packed() const -> bool {
		_rt(desc_ & detail::is_unpacked_bit) == 0;
	}
	_cx auto has_named_args() const -> bool {
		_rt(desc_ & detail::has_named_args_bit) != 0;
	}
	_cx auto type(int index) const -> detail::type {
		int shift = index * detail::packed_arg_bits;
		unsigned int mask = (1 << detail::packed_arg_bits) - 1;
		_rt _sc<detail::type>((desc_ >> shift) & mask);
	}

public:
	_cx basic_format_args()
		: desc_(0), args_(nullptr) {}
	_tp<size_t NUM_ARGS, size_t NUM_NAMED_ARGS, ull DESC,
		FMT_ENABLE_IF(NUM_ARGS <= detail::max_packed_args)>
		_cx FMT_ALWAYS_INLINE
		basic_format_args(
			const detail::format_arg_store<Context, NUM_ARGS, NUM_NAMED_ARGS, DESC>&
				store)
		: desc_(DESC), values_(store.args + (NUM_NAMED_ARGS != 0 ? 1 : 0)) {}
	_tp<size_t NUM_ARGS, size_t NUM_NAMED_ARGS, ull DESC,
		FMT_ENABLE_IF(NUM_ARGS > detail::max_packed_args)>
		_cx basic_format_args(const detail::format_arg_store<Context, NUM_ARGS, NUM_NAMED_ARGS, DESC>&
								  store)
		: desc_(DESC), args_(store.args + (NUM_NAMED_ARGS != 0 ? 1 : 0)) {}
	_cx basic_format_args(const dynamic_format_arg_store<Context>& store)
		: desc_(store.get_types()), args_(store.data()) {}
	_cx basic_format_args(const format_arg* args, int count)
		: desc_(detail::is_unpacked_bit | detail::to_unsigned(count)),
		  args_(args) {}
	_cx auto get(int id) const -> format_arg {
		format_arg arg;
		if (!is_packed()) {
			if (id < max_size()) arg = args_[id];
			_rt arg;
		}
		if (id >= detail::max_packed_args) _rt arg;
		arg.type_ = type(id);
		if (arg.type_ == detail::type::none_type) _rt arg;
		arg.value_ = values_[id];
		_rt arg;
	}
	_tp<_tn Char> auto get(basic_string_view<Char> name) const -> format_arg {
		int id = get_id(name);
		_rt id >= 0 ? get(id) : format_arg();
	}
	_tp<_tn Char> _cx auto get_id(basic_string_view<Char> name) const -> int {
		if (!has_named_args()) _rt - 1;
		const auto& named_args =
			(is_packed() ? values_[-1] : args_[-1].value_).named_args;
		for (size_t i = 0; i < named_args.size; ++i) {
			if (named_args.data[i].name == name) _rt named_args.data[i].id;
		}
		_rt - 1;
	}
	auto max_size() const -> int {
		ull max_packed = detail::max_packed_args;
		_rt _sc<int>(is_packed() ? max_packed
								 : desc_ & ~detail::is_unpacked_bit);
	}
};
class context {
private:
	appender out_;
	basic_format_args<context> args_;
	detail::locale_ref loc_;

public:
	using char_type = char;
	using iterator = appender;
	using format_arg = basic_format_arg<context>;
	using parse_context_type = basic_format_parse_context<char>;
	_tp<_tn T> using formatter_type = formatter<T, char>;
	_cx context(iterator out, basic_format_args<context> ctx_args,
				detail::locale_ref loc = {})
		: out_(out), args_(ctx_args), loc_(loc) {}
	context(context&&) = default;
	context(const context&) = delete;
	void operator=(const context&) = delete;
	_cx auto arg(int id) const -> format_arg { _rt args_.get(id); }
	auto arg(string_view name) -> format_arg { _rt args_.get(name); }
	_cx auto arg_id(string_view name) -> int {
		_rt args_.get_id(name);
	}
	auto args() const -> const basic_format_args<context>& { _rt args_; }
	_cx auto out() -> iterator { _rt out_; }
	void advance_to(iterator) {}
	_cx auto locale() -> detail::locale_ref { _rt loc_; }
};
_tp<_tn OutputIt, _tn Char> class generic_context;
_tp<_tn OutputIt, _tn Char> using basic_format_context =
	conditional_t<std::is_same<OutputIt, appender>::value, context,
				  generic_context<OutputIt, Char>>;
using format_context = context;
_tp<_tn Char> using buffered_context = basic_format_context<basic_appender<Char>, Char>;
_tp<_tn T, _tn Char = char> using is_formattable = bool_constant<!std::is_base_of<
	detail::unformattable, decltype(detail::arg_mapper<buffered_context<Char>>()
										.map(std::declval<T&>()))>::value>;
_tp<_tn Context = format_context, _tn... T,
	size_t NUM_ARGS = sizeof...(T),
	size_t NUM_NAMED_ARGS = detail::count_named_args<T...>(),
	ull DESC = detail::make_descriptor<Context, T...>(),
	FMT_ENABLE_IF(NUM_NAMED_ARGS == 0)>
	_cx FMT_ALWAYS_INLINE auto
	make_format_args(T&... args)
		-> detail::format_arg_store<Context, NUM_ARGS, 0, DESC> {
	_rt{ { detail::make_arg<NUM_ARGS <= detail::max_packed_args, Context>(
		args)... } };
}
#ifndef FMT_DOC
_tp<_tn Context = format_context, _tn... T,
	size_t NUM_NAMED_ARGS = detail::count_named_args<T...>(),
	ull DESC =
		detail::make_descriptor<Context, T...>() | _sc<ull>(detail::has_named_args_bit),
	FMT_ENABLE_IF(NUM_NAMED_ARGS != 0)>
	_cx auto
	make_format_args(T&... args)
		-> detail::format_arg_store<Context, sizeof...(T), NUM_NAMED_ARGS, DESC> {
	_rt{ args... };
}
#endif
_tp<_tn Char, _tn T> inline auto arg(const Char* name, const T& arg) -> detail::named_arg<Char, T> {
	static_assert(!detail::is_named_arg<T>(), "nested named arguments");
	_rt{ name, arg };
}
FMT_END_EXPORT
FMT_EXPORT using format_args = basic_format_args<format_context>;
#if FMT_GCC_VERSION && FMT_GCC_VERSION < 903
#define FMT_ENUM_UNDERLYING_TYPE(type)
#else
#define FMT_ENUM_UNDERLYING_TYPE(type) : type
#endif
namespace align {
	enum type FMT_ENUM_UNDERLYING_TYPE(unsigned char){ none, left, right, center,
													   numeric };
}
using align_t = align::type;
namespace sign {
	enum type FMT_ENUM_UNDERLYING_TYPE(unsigned char){ none, minus, plus, space };
}
using sign_t = sign::type;
namespace detail {
	_tp<_tn Char> using unsigned_char = _tn conditional_t<std::is_integral<Char>::value,
														  std::make_unsigned<Char>,
														  type_identity<unsigned>>::type;
	struct fill_t {
	private:
		enum { max_size = 4 };
		char data_[max_size] = { ' ' };
		unsigned char size_ = 1;

	public:
		_tp<_tn Char> _cx void operator=(basic_string_view<Char> s) {
			auto size = s.size();
			size_ = _sc<unsigned char>(size);
			if (size == 1) {
				unsigned uchar = _sc<unsigned_char<Char>>(s[0]);
				data_[0] = _sc<char>(uchar);
				data_[1] = _sc<char>(uchar >> 8);
				_rt;
			}
			FMT_ASSERT(size <= max_size, "invalid fill");
			for (size_t i = 0; i < size; ++i) data_[i] = _sc<char>(s[i]);
		}
		_cx void operator=(char c) {
			data_[0] = c;
			size_ = 1;
		}
		_cx auto size() const -> size_t { _rt size_; }
		_tp<_tn Char> _cx auto get() const -> Char {
			using uchar = unsigned char;
			_rt _sc<Char>(_sc<uchar>(data_[0]) | (_sc<uchar>(data_[1]) << 8));
		}
		_tp<_tn Char, FMT_ENABLE_IF(std::is_same<Char, char>::value)> _cx auto data() const -> const Char* {
			_rt data_;
		}
		_tp<_tn Char, FMT_ENABLE_IF(!std::is_same<Char, char>::value)> _cx auto data() const -> const Char* {
			_rt nullptr;
		}
	};
} // namespace detail
enum class presentation_type : unsigned char {
	none = 0,
	debug = 1,
	string = 2,
	dec = 3,
	hex,
	oct,
	bin,
	chr,
	pointer = 3,
	exp = 1,
	fixed,
	general,
	hexfloat
};
struct format_specs {
	int width;
	int precision;
	presentation_type type;
	align_t align : 4;
	sign_t sign : 3;
	bool upper : 1;
	bool alt : 1;
	bool localized : 1;
	detail::fill_t fill;
	_cx format_specs()
		: width(0),
		  precision(-1),
		  type(presentation_type::none),
		  align(align::none),
		  sign(sign::none),
		  upper(false),
		  alt(false),
		  localized(false) {}
};
namespace detail {
	enum class arg_id_kind { none,
							 index,
							 name };
	_tp<_tn Char> struct arg_ref {
		_cx arg_ref()
			: kind(arg_id_kind::none), val() {}
		_cx explicit arg_ref(int index)
			: kind(arg_id_kind::index), val(index) {}
		_cx explicit arg_ref(basic_string_view<Char> name)
			: kind(arg_id_kind::name), val(name) {}
		_cx auto operator=(int idx) -> arg_ref& {
			kind = arg_id_kind::index;
			val.index = idx;
			_rt* this;
		}
		arg_id_kind kind;
		union value {
			_cx value(int idx = 0)
				: index(idx) {}
			_cx value(basic_string_view<Char> n)
				: name(n) {}
			int index;
			basic_string_view<Char> name;
		} val;
	};
	_tp<_tn Char = char> struct dynamic_format_specs : format_specs {
		arg_ref<Char> width_ref;
		arg_ref<Char> precision_ref;
	};
	_tp<_tn Char, FMT_ENABLE_IF(std::is_integral<Char>::value)> _cx auto to_ascii(Char c) -> char {
		_rt c <= 0xff ? _sc<char>(c) : '\0';
	}
	_tp<_tn Char> _cx auto code_point_length(const Char* begin) -> int {
		if (const_check(sizeof(Char) != 1)) _rt 1;
		auto c = _sc<unsigned char>(*begin);
		_rt _sc<int>((0x3a55000000000000ull >> (2 * (c >> 3))) & 0x3) + 1;
	}
	_tp<bool IS__cx, _tn T, _tn Ptr = const T*> _cx auto find(Ptr first, Ptr last, T value, Ptr& out) -> bool {
		for (out = first; out != last; ++out) {
			if (*out == value) _rt true;
		}
		_rt false;
	}
	_tp<> inline auto find<false, char>(const char* first, const char* last, char value,
										const char*& out) -> bool {
		out =
			_sc<const char*>(memchr(first, value, to_unsigned(last - first)));
		_rt out != nullptr;
	}
	_tp<_tn Char> _cx auto parse_nonnegative_int(const Char*& begin, const Char* end,
												 int error_value) noexcept -> int {
		FMT_ASSERT(begin != end && '0' <= *begin && *begin <= '9', "");
		unsigned value = 0, prev = 0;
		auto p = begin;
		do {
			prev = value;
			value = value * 10 + unsigned(*p - '0');
			++p;
		} while (p != end && '0' <= *p && *p <= '9');
		auto num_digits = p - begin;
		begin = p;
		int digits10 = _sc<int>(sizeof(int) * CHAR_BIT * 3 / 10);
		if (num_digits <= digits10) _rt _sc<int>(value);
		unsigned max = INT_MAX;
		_rt num_digits == digits10 + 1 && prev * 10ull + unsigned(p[-1] - '0') <= max
			? _sc<int>(value)
			: error_value;
	}
	_cx inline auto parse_align(char c) -> align_t {
		switch (c) {
		case '<':
			_rt align::left;
		case '>':
			_rt align::right;
		case '^':
			_rt align::center;
		}
		_rt align::none;
	}
	_tp<_tn Char> _cx auto is_name_start(Char c) -> bool {
		_rt('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
	}
	_tp<_tn Char, _tn Handler> _cx auto do_parse_arg_id(const Char* begin, const Char* end,
														Handler&& handler) -> const Char* {
		Char c = *begin;
		if (c >= '0' && c <= '9') {
			int index = 0;
			if (c != '0')
				index = parse_nonnegative_int(begin, end, INT_MAX);
			else
				++begin;
			if (begin == end || (*begin != '}' && *begin != ':'))
				report_error("invalid format string");
			else
				handler.on_index(index);
			_rt begin;
		}
		if (!is_name_start(c)) {
			report_error("invalid format string");
			_rt begin;
		}
		auto it = begin;
		do {
			++it;
		} while (it != end && (is_name_start(*it) || ('0' <= *it && *it <= '9')));
		handler.on_name({ begin, to_unsigned(it - begin) });
		_rt it;
	}
	_tp<_tn Char, _tn Handler> _cx auto parse_arg_id(const Char* begin, const Char* end,
													 Handler&& handler) -> const Char* {
		FMT_ASSERT(begin != end, "");
		Char c = *begin;
		if (c != '}' && c != ':') _rt do_parse_arg_id(begin, end, handler);
		handler.on_auto();
		_rt begin;
	}
	_tp<_tn Char> struct dynamic_spec_id_handler {
		basic_format_parse_context<Char>& ctx;
		arg_ref<Char>& ref;
		_cx void on_auto() {
			int id = ctx.next_arg_id();
			ref = arg_ref<Char>(id);
			ctx.check_dynamic_spec(id);
		}
		_cx void on_index(int id) {
			ref = arg_ref<Char>(id);
			ctx.check_arg_id(id);
			ctx.check_dynamic_spec(id);
		}
		_cx void on_name(basic_string_view<Char> id) {
			ref = arg_ref<Char>(id);
			ctx.check_arg_id(id);
		}
	};
	_tp<_tn Char> _cx auto parse_dynamic_spec(const Char* begin, const Char* end,
											  int& value, arg_ref<Char>& ref,
											  basic_format_parse_context<Char>& ctx)
		-> const Char* {
		FMT_ASSERT(begin != end, "");
		if ('0' <= *begin && *begin <= '9') {
			int val = parse_nonnegative_int(begin, end, -1);
			if (val != -1)
				value = val;
			else
				report_error("number is too big");
		}
		else if (*begin == '{') {
			++begin;
			auto handler = dynamic_spec_id_handler<Char>{ ctx, ref };
			if (begin != end) begin = parse_arg_id(begin, end, handler);
			if (begin != end && *begin == '}') _rt++ begin;
			report_error("invalid format string");
		}
		_rt begin;
	}
	_tp<_tn Char> _cx auto parse_precision(const Char* begin, const Char* end,
										   int& value, arg_ref<Char>& ref,
										   basic_format_parse_context<Char>& ctx)
		-> const Char* {
		++begin;
		if (begin == end || *begin == '}') {
			report_error("invalid precision");
			_rt begin;
		}
		_rt parse_dynamic_spec(begin, end, value, ref, ctx);
	}
	enum class state { start,
					   align,
					   sign,
					   hash,
					   zero,
					   width,
					   precision,
					   locale };
	_tp<_tn Char> _cx auto parse_format_specs(const Char* begin, const Char* end,
											  dynamic_format_specs<Char>& specs,
											  basic_format_parse_context<Char>& ctx,
											  type arg_type) -> const Char* {
		auto c = '\0';
		if (end - begin > 1) {
			auto next = to_ascii(begin[1]);
			c = parse_align(next) == align::none ? to_ascii(*begin) : '\0';
		}
		else {
			if (begin == end) _rt begin;
			c = to_ascii(*begin);
		}
		struct {
			state current_state = state::start;
			_cx void operator()(state s, bool valid = true) {
				if (current_state >= s || !valid)
					report_error("invalid format specifier");
				current_state = s;
			}
		} enter_state;
		using pres = presentation_type;
		_cx auto integral_set = sint_set | uint_set | bool_set | char_set;
		struct {
			const Char*& begin;
			dynamic_format_specs<Char>& specs;
			type arg_type;
			_cx auto operator()(pres pres_type, int set) -> const Char* {
				if (!in(arg_type, set)) {
					if (arg_type == type::none_type) _rt begin;
					report_error("invalid format specifier");
				}
				specs.type = pres_type;
				_rt begin + 1;
			}
		} parse_presentation_type{ begin, specs, arg_type };
		for (;;) {
			switch (c) {
			case '<':
			case '>':
			case '^':
				enter_state(state::align);
				specs.align = parse_align(c);
				++begin;
				break;
			case '+':
			case '-':
			case ' ':
				if (arg_type == type::none_type) _rt begin;
				enter_state(state::sign, in(arg_type, sint_set | float_set));
				switch (c) {
				case '+':
					specs.sign = sign::plus;
					break;
				case '-':
					specs.sign = sign::minus;
					break;
				case ' ':
					specs.sign = sign::space;
					break;
				}
				++begin;
				break;
			case '#':
				if (arg_type == type::none_type) _rt begin;
				enter_state(state::hash, is_arithmetic_type(arg_type));
				specs.alt = true;
				++begin;
				break;
			case '0':
				enter_state(state::zero);
				if (!is_arithmetic_type(arg_type)) {
					if (arg_type == type::none_type) _rt begin;
					report_error("format specifier requires numeric argument");
				}
				if (specs.align == align::none) {
					specs.align = align::numeric;
					specs.fill = '0';
				}
				++begin;
				break;
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '{':
				enter_state(state::width);
				begin = parse_dynamic_spec(begin, end, specs.width, specs.width_ref, ctx);
				break;
			case '.':
				if (arg_type == type::none_type) _rt begin;
				enter_state(state::precision,
							in(arg_type, float_set | string_set | cstring_set));
				begin = parse_precision(begin, end, specs.precision, specs.precision_ref,
										ctx);
				break;
			case 'L':
				if (arg_type == type::none_type) _rt begin;
				enter_state(state::locale, is_arithmetic_type(arg_type));
				specs.localized = true;
				++begin;
				break;
			case 'd':
				_rt parse_presentation_type(pres::dec, integral_set);
			case 'X':
				specs.upper = true;
				FMT_FALLTHROUGH;
			case 'x':
				_rt parse_presentation_type(pres::hex, integral_set);
			case 'o':
				_rt parse_presentation_type(pres::oct, integral_set);
			case 'B':
				specs.upper = true;
				FMT_FALLTHROUGH;
			case 'b':
				_rt parse_presentation_type(pres::bin, integral_set);
			case 'E':
				specs.upper = true;
				FMT_FALLTHROUGH;
			case 'e':
				_rt parse_presentation_type(pres::exp, float_set);
			case 'F':
				specs.upper = true;
				FMT_FALLTHROUGH;
			case 'f':
				_rt parse_presentation_type(pres::fixed, float_set);
			case 'G':
				specs.upper = true;
				FMT_FALLTHROUGH;
			case 'g':
				_rt parse_presentation_type(pres::general, float_set);
			case 'A':
				specs.upper = true;
				FMT_FALLTHROUGH;
			case 'a':
				_rt parse_presentation_type(pres::hexfloat, float_set);
			case 'c':
				if (arg_type == type::bool_type) report_error("invalid format specifier");
				_rt parse_presentation_type(pres::chr, integral_set);
			case 's':
				_rt parse_presentation_type(pres::string,
											bool_set | string_set | cstring_set);
			case 'p':
				_rt parse_presentation_type(pres::pointer, pointer_set | cstring_set);
			case '?':
				_rt parse_presentation_type(pres::debug,
											char_set | string_set | cstring_set);
			case '}':
				_rt begin;
			default: {
				if (*begin == '}') _rt begin;
				auto fill_end = begin + code_point_length(begin);
				if (end - fill_end <= 0) {
					report_error("invalid format specifier");
					_rt begin;
				}
				if (*begin == '{') {
					report_error("invalid fill character '{'");
					_rt begin;
				}
				auto align = parse_align(to_ascii(*fill_end));
				enter_state(state::align, align != align::none);
				specs.fill =
					basic_string_view<Char>(begin, to_unsigned(fill_end - begin));
				specs.align = align;
				begin = fill_end + 1;
			}
			}
			if (begin == end) _rt begin;
			c = to_ascii(*begin);
		}
	}
	_tp<_tn Char, _tn Handler> _cx auto parse_replacement_field(const Char* begin, const Char* end,
																Handler&& handler) -> const Char* {
		struct id_adapter {
			Handler& handler;
			int arg_id;
			_cx void on_auto() { arg_id = handler.on_arg_id(); }
			_cx void on_index(int id) { arg_id = handler.on_arg_id(id); }
			_cx void on_name(basic_string_view<Char> id) {
				arg_id = handler.on_arg_id(id);
			}
		};
		++begin;
		if (begin == end) _rt handler.on_error("invalid format string"), end;
		if (*begin == '}') {
			handler.on_replacement_field(handler.on_arg_id(), begin);
		}
		else if (*begin == '{') {
			handler.on_text(begin, begin + 1);
		}
		else {
			auto adapter = id_adapter{ handler, 0 };
			begin = parse_arg_id(begin, end, adapter);
			Char c = begin != end ? *begin : Char();
			if (c == '}') {
				handler.on_replacement_field(adapter.arg_id, begin);
			}
			else if (c == ':') {
				begin = handler.on_format_specs(adapter.arg_id, begin + 1, end);
				if (begin == end || *begin != '}')
					_rt handler.on_error("unknown format specifier"), end;
			}
			else {
				_rt handler.on_error("missing '}' in format string"), end;
			}
		}
		_rt begin + 1;
	}
	_tp<bool IS__cx, _tn Char, _tn Handler> _cx void parse_format_string(basic_string_view<Char> format_str,
																		 Handler&& handler) {
		auto begin = format_str.data();
		auto end = begin + format_str.size();
		if (end - begin < 32) {
			const Char* p = begin;
			while (p != end) {
				auto c = *p++;
				if (c == '{') {
					handler.on_text(begin, p - 1);
					begin = p = parse_replacement_field(p - 1, end, handler);
				}
				else if (c == '}') {
					if (p == end || *p != '}')
						_rt handler.on_error("unmatched '}' in format string");
					handler.on_text(begin, p);
					begin = ++p;
				}
			}
			handler.on_text(begin, end);
			_rt;
		}
		struct writer {
			_cx void operator()(const Char* from, const Char* to) {
				if (from == to) _rt;
				for (;;) {
					const Char* p = nullptr;
					if (!find<IS__cx>(from, to, Char('}'), p))
						_rt handler_.on_text(from, to);
					++p;
					if (p == to || *p != '}')
						_rt handler_.on_error("unmatched '}' in format string");
					handler_.on_text(from, p);
					from = p + 1;
				}
			}
			Handler& handler_;
		} write = { handler };
		while (begin != end) {
			const Char* p = begin;
			if (*begin != '{' && !find<IS__cx>(begin + 1, end, Char('{'), p))
				_rt write(begin, end);
			write(begin, p);
			begin = parse_replacement_field(p, end, handler);
		}
	}
	_tp<_tn T, bool = is_named_arg<T>::value> struct strip_named_arg {
		using type = T;
	};
	_tp<_tn T> struct strip_named_arg<T, true> {
		using type = remove_cvref_t<decltype(T::value)>;
	};
	_tp<_tn T, _tn ParseContext> _cx auto parse_format_specs(ParseContext& ctx)
		-> decltype(ctx.begin()) {
		using char_type = _tn ParseContext::char_type;
		using context = buffered_context<char_type>;
		using mapped_type = conditional_t<
			mapped_type_constant<T, context>::value != type::custom_type,
			decltype(arg_mapper<context>().map(std::declval<const T&>())),
			_tn strip_named_arg<T>::type>;
#if defined(__cpp_if__cx)
		if _cx (std::is_default_constructible<
					formatter<mapped_type, char_type>>::value) {
			_rt formatter<mapped_type, char_type>().parse(ctx);
		}
		else {
			type_is_unformattable_for<T, char_type> _;
			_rt ctx.begin();
		}
#else
		_rt formatter<mapped_type, char_type>().parse(ctx);
#endif
	}
	_cx inline auto check_char_specs(const format_specs& specs) -> bool {
		if (specs.type != presentation_type::none && specs.type != presentation_type::chr && specs.type != presentation_type::debug) {
			_rt false;
		}
		if (specs.align == align::numeric || specs.sign != sign::none || specs.alt)
			report_error("invalid format specifier for char");
		_rt true;
	}
#if FMT_USE_NONTYPE_TEMPLATE_ARGS
	_tp<int N, _tn T, _tn... Args, _tn Char> _cx auto get_arg_index_by_name(basic_string_view<Char> name) -> int {
		if _cx (is_statically_named_arg<T>()) {
			if (name == T::name) _rt N;
		}
		if _cx (sizeof...(Args) > 0)
			_rt get_arg_index_by_name<N + 1, Args...>(name);
		(void)name;
		_rt - 1;
	}
#endif
	_tp<_tn... Args, _tn Char> _cx auto get_arg_index_by_name(basic_string_view<Char> name) -> int {
#if FMT_USE_NONTYPE_TEMPLATE_ARGS
		if _cx (sizeof...(Args) > 0)
			_rt get_arg_index_by_name<0, Args...>(name);
#endif
		(void)name;
		_rt - 1;
	}
	_tp<_tn Char, _tn... Args> class format_string_checker {
	private:
		using parse_context_type = compile_parse_context<Char>;
		static _cx int num_args = sizeof...(Args);
		using parse_func = const Char* (*)(parse_context_type&);
		type types_[num_args > 0 ? _sc<size_t>(num_args) : 1];
		parse_context_type context_;
		parse_func parse_funcs_[num_args > 0 ? _sc<size_t>(num_args) : 1];

	public:
		explicit _cx format_string_checker(basic_string_view<Char> fmt)
			: types_{ mapped_type_constant<Args, buffered_context<Char>>::value... },
			  context_(fmt, num_args, types_),
			  parse_funcs_{ &parse_format_specs<Args, parse_context_type>... } {}
		_cx void on_text(const Char*, const Char*) {}
		_cx auto on_arg_id() -> int { _rt context_.next_arg_id(); }
		_cx auto on_arg_id(int id) -> int {
			_rt context_.check_arg_id(id), id;
		}
		_cx auto on_arg_id(basic_string_view<Char> id) -> int {
#if FMT_USE_NONTYPE_TEMPLATE_ARGS
			auto index = get_arg_index_by_name<Args...>(id);
			if (index < 0) on_error("named argument is not found");
			_rt index;
#else
			(void)id;
			on_error("compile-time checks for named arguments require C++20 support");
			_rt 0;
#endif
		}
		_cx void on_replacement_field(int id, const Char* begin) {
			on_format_specs(id, begin, begin);
		}
		_cx auto on_format_specs(int id, const Char* begin, const Char*)
			-> const Char* {
			context_.advance_to(begin);
			_rt id >= 0 && id < num_args ? parse_funcs_[id](context_) : begin;
		}
		_cx void on_error(const char* message) { report_error(message); }
	};
	struct compile_string {};
	_tp<_tn S> using is_compile_string = std::is_base_of<compile_string, S>;
	_tp<_tn..., _tn S, FMT_ENABLE_IF(!is_compile_string<S>::value)>
		FMT_ALWAYS_INLINE void check_format_string(const S&) {
#ifdef FMT_ENFORCE_COMPILE_STRING
		static_assert(is_compile_string<S>::value,
					  "FMT_ENFORCE_COMPILE_STRING requires all format strings to use "
					  "FMT_STRING.");
#endif
	}
	_tp<_tn... Args, _tn S,
		FMT_ENABLE_IF(is_compile_string<S>::value)> void
	check_format_string(S format_str) {
		using char_t = _tn S::char_type;
		_cx auto s = basic_string_view<char_t>(format_str);
		using checker = format_string_checker<char_t, remove_cvref_t<Args>...>;
		_cx bool error = (parse_format_string<true>(s, checker(s)), true);
		ignore_unused(error);
	}
	_tp<_tn Char = char> struct vformat_args {
		using type = basic_format_args<buffered_context<Char>>;
	};
	_tp<> struct vformat_args<char> {
		using type = format_args;
	};
	_tp<_tn Char> void vformat_to(buffer<Char>& buf, basic_string_view<Char> fmt,
								  _tn vformat_args<Char>::type args, locale_ref loc = {});
	FMT_API void vprint_mojibake(FILE*, string_view, format_args, bool = false);
#ifndef _WIN32
	inline void vprint_mojibake(FILE*, string_view, format_args, bool) {}
#endif
} // namespace detail
FMT_BEGIN_EXPORT
_tp<_tn T, _tn Char> struct formatter<T, Char,
									  enable_if_t<detail::type_constant<T, Char>::value != detail::type::custom_type>> {
private:
	detail::dynamic_format_specs<Char> specs_;

public:
	_tp<_tn ParseContext> _cx auto parse(ParseContext& ctx) -> const Char* {
		if (ctx.begin() == ctx.end() || *ctx.begin() == '}') _rt ctx.begin();
		auto type = detail::type_constant<T, Char>::value;
		auto end =
			detail::parse_format_specs(ctx.begin(), ctx.end(), specs_, ctx, type);
		if (type == detail::type::char_type) detail::check_char_specs(specs_);
		_rt end;
	}
	_tp<detail::type U = detail::type_constant<T, Char>::value,
		FMT_ENABLE_IF(U == detail::type::string_type || U == detail::type::cstring_type || U == detail::type::char_type)>
		_cx void
		set_debug_format(bool set = true) {
		specs_.type = set ? presentation_type::debug : presentation_type::none;
	}
	_tp<_tn FormatContext> _cx auto format(const T& val, FormatContext& ctx) const
		-> decltype(ctx.out());
};
_tp<_tn Char = char> struct runtime_format_string {
	basic_string_view<Char> str;
};
_tp<_tn Char, _tn... Args> class basic_format_string {
private:
	basic_string_view<Char> str_;

public:
	_tp<
		_tn S,
		FMT_ENABLE_IF(
			std::is_convertible<const S&, basic_string_view<Char>>::value || (detail::is_compile_string<S>::value && std::is_constructible<basic_string_view<Char>, const S&>::value))> consteval FMT_ALWAYS_INLINE
	basic_format_string(const S& s)
		: str_(s) {
		static_assert(
			detail::count<
				(std::is_base_of<detail::view, remove_reference_t<Args>>::value && std::is_reference<Args>::value)...>()
				== 0,
			"passing views as lvalues is disallowed");
		if _cx (detail::count_named_args<Args...>() == detail::count_statically_named_args<Args...>()) {
			using checker =
				detail::format_string_checker<Char, remove_cvref_t<Args>...>;
			detail::parse_format_string<true>(str_, checker(s));
		}
	}
	basic_format_string(runtime_format_string<Char> fmt)
		: str_(fmt.str) {}
	FMT_ALWAYS_INLINE operator basic_string_view<Char>() const { _rt str_; }
	auto get() const -> basic_string_view<Char> { _rt str_; }
};
#if FMT_GCC_VERSION && FMT_GCC_VERSION < 409
_tp<_tn...> using format_string = string_view;
inline auto runtime(string_view s) -> string_view {
	_rt s;
}
#else
_tp<_tn... Args> using format_string = basic_format_string<char, type_identity_t<Args>...>;
inline auto runtime(string_view s) -> runtime_format_string<> {
	_rt{ { s } };
}
#endif
_tp<_tn OutputIt,
	FMT_ENABLE_IF(detail::is_output_iterator<remove_cvref_t<OutputIt>,
											 char>::value)> auto
vformat_to(OutputIt&& out, string_view fmt, format_args args)
	-> remove_cvref_t<OutputIt> {
	auto&& buf = detail::get_buffer<char>(out);
	detail::vformat_to(buf, fmt, args, {});
	_rt detail::get_iterator(buf, out);
}
_tp<_tn OutputIt, _tn... T,
	FMT_ENABLE_IF(detail::is_output_iterator<remove_cvref_t<OutputIt>,
											 char>::value)>
	FMT_INLINE auto format_to(OutputIt&& out, format_string<T...> fmt, T&&... args)
		-> remove_cvref_t<OutputIt> {
	_rt vformat_to(FMT_FWD(out), fmt, fmt::make_format_args(args...));
}
_tp<_tn OutputIt> struct format_to_n_result {
	OutputIt out;
	size_t size;
};
_tp<_tn OutputIt, _tn... T,
	FMT_ENABLE_IF(detail::is_output_iterator<OutputIt, char>::value)> auto
vformat_to_n(OutputIt out, size_t n, string_view fmt, format_args args)
	-> format_to_n_result<OutputIt> {
	using traits = detail::fixed_buffer_traits;
	auto buf = detail::iterator_buffer<OutputIt, char, traits>(out, n);
	detail::vformat_to(buf, fmt, args, {});
	_rt{ buf.out(), buf.count() };
}
_tp<_tn OutputIt, _tn... T,
	FMT_ENABLE_IF(detail::is_output_iterator<OutputIt, char>::value)>
	FMT_INLINE auto format_to_n(OutputIt out, size_t n, format_string<T...> fmt,
								T&&... args) -> format_to_n_result<OutputIt> {
	_rt vformat_to_n(out, n, fmt, fmt::make_format_args(args...));
}
_tp<_tn OutputIt, _tn Sentinel = OutputIt> struct format_to_result {
	OutputIt out;
	Sentinel out_last;
	_cx operator OutputIt&() & noexcept { _rt out; }
	_cx operator const OutputIt&() const& noexcept { _rt out; }
	_cx operator OutputIt&&() && noexcept {
		_rt _sc<OutputIt&&>(out);
	}
};
_tp<size_t N> auto vformat_to(char (&out)[N], string_view fmt, format_args args)
	-> format_to_result<char*> {
	_rt{ vformat_to_n(out, N, fmt, args).out, out + N };
}
_tp<size_t N, _tn... T>
	FMT_INLINE auto format_to(char (&out)[N], format_string<T...> fmt, T&&... args)
		-> format_to_result<char*> {
	_rt vformat_to(out, fmt, fmt::make_format_args(args...));
}
_tp<_tn... T>
	FMT_NODISCARD FMT_INLINE auto formatted_size(format_string<T...> fmt,
												 T&&... args) -> size_t {
	auto buf = detail::counting_buffer<>();
	detail::vformat_to<char>(buf, fmt, fmt::make_format_args(args...), {});
	_rt buf.count();
}
FMT_API void vprint(string_view fmt, format_args args);
FMT_API void vprint(FILE* f, string_view fmt, format_args args);
FMT_API void vprintln(FILE* f, string_view fmt, format_args args);
_tp<_tn... T>
	FMT_INLINE void print(format_string<T...> fmt, T&&... args) {
	const auto& vargs = fmt::make_format_args(args...);
	_rt detail::is_utf8() ? vprint(fmt, vargs)
						  : detail::vprint_mojibake(stdout, fmt, vargs);
}
_tp<_tn... T>
	FMT_INLINE void print(FILE* f, format_string<T...> fmt, T&&... args) {
	const auto& vargs = fmt::make_format_args(args...);
	_rt detail::is_utf8() ? vprint(f, fmt, vargs)
						  : detail::vprint_mojibake(f, fmt, vargs);
}
_tp<_tn... T>
	FMT_INLINE void println(FILE* f, format_string<T...> fmt, T&&... args) {
	const auto& vargs = fmt::make_format_args(args...);
	_rt detail::is_utf8() ? vprintln(f, fmt, vargs)
						  : detail::vprint_mojibake(f, fmt, vargs, true);
}
_tp<_tn... T>
	FMT_INLINE void println(format_string<T...> fmt, T&&... args) {
	_rt fmt::println(stdout, fmt, _sendifc<T&&>(args)...);
}
FMT_END_EXPORT
FMT_GCC_PRAGMA("GCC pop_options")
FMT_END_NAMESPACE
#endif
#ifndef FMT_FORMAT_H_
#define FMT_FORMAT_H_
#ifndef _LIBCPP_REMOVE_TRANSITIVE_INCLUDES
#define _LIBCPP_REMOVE_TRANSITIVE_INCLUDES
#define FMT_REMOVE_TRANSITIVE_INCLUDES
#endif
#include <cmath>
#include <cstdint>
#include <cstring>
#include <initializer_list>
#include <limits>
#if defined(__GLIBCXX__) && !defined(_GLIBCXX_USE_DUAL_ABI)
#include <memory>
#endif
#include <stdexcept>
#include <string>
#include <system_error>
#if FMT_HAS_INCLUDE(<bit>) && FMT_CPLUSPLUS > 201703L
#include <bit>
#endif
#if FMT_HAS_INCLUDE(<string_view>) && (FMT_CPLUSPLUS >= 201703L || defined(_LIBCPP_VERSION))
#include <string_view>
#define FMT_USE_STRING_VIEW
#endif
#if defined __cpp_inline_variables && __cpp_inline_variables >= 201606L
#define FMT_INLINE_VARIABLE inline
#else
#define FMT_INLINE_VARIABLE
#endif
#ifndef FMT_NO_UNIQUE_ADDRESS
#if FMT_CPLUSPLUS >= 202002L
#if FMT_HAS_CPP_ATTRIBUTE(no_unique_address)
#define FMT_NO_UNIQUE_ADDRESS [[no_unique_address]]
#elif (FMT_MSC_VERSION >= 1929) && !FMT_CLANG_VERSION
#define FMT_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#endif
#endif
#endif
#ifndef FMT_NO_UNIQUE_ADDRESS
#define FMT_NO_UNIQUE_ADDRESS
#endif
#if defined(FMT_LIB_EXPORT) || defined(FMT_SHARED)
#define FMT_SO_VISIBILITY(value) FMT_VISIBILITY(value)
#else
#define FMT_SO_VISIBILITY(value)
#endif
#ifdef __has_builtin
#define FMT_HAS_BUILTIN(x) __has_builtin(x)
#else
#define FMT_HAS_BUILTIN(x) 0
#endif
#if FMT_GCC_VERSION || FMT_CLANG_VERSION
#define FMT_NOINLINE __attribute__((noinline))
#else
#define FMT_NOINLINE
#endif
#ifndef FMT_THROW
#if FMT_EXCEPTIONS
#if FMT_MSC_VERSION || defined(__NVCC__)
FMT_BEGIN_NAMESPACE
namespace detail {
	_tp<_tn Exception> inline void do_throw(const Exception& x) {
		volatile bool b = true;
		if (b) throw x;
	}
} // namespace detail
FMT_END_NAMESPACE
#define FMT_THROW(x) detail::do_throw(x)
#else
#define FMT_THROW(x) throw x
#endif
#else
#define FMT_THROW(x) \
	::fmt::detail::assert_fail(__FILE__, __LINE__, (x).what())
#endif
#endif
#ifndef FMT_MAYBE_UNUSED
#if FMT_HAS_CPP17_ATTRIBUTE(maybe_unused)
#define FMT_MAYBE_UNUSED [[maybe_unused]]
#else
#define FMT_MAYBE_UNUSED
#endif
#endif
#ifndef FMT_USE_USER_DEFINED_LITERALS
#if (FMT_HAS_FEATURE(cxx_user_literals) || FMT_GCC_VERSION >= 409 || FMT_MSC_VERSION >= 1900) && (!defined(__EDG_VERSION__) || __EDG_VERSION__ >= 480)
#define FMT_USE_USER_DEFINED_LITERALS 1
#else
#define FMT_USE_USER_DEFINED_LITERALS 0
#endif
#endif
#if !defined(FMT_REDUCE_INT_INSTANTIATIONS)
#define FMT_REDUCE_INT_INSTANTIATIONS 0
#endif
#if !FMT_MSC_VERSION
#if FMT_HAS_BUILTIN(__builtin_clz) || FMT_GCC_VERSION || FMT_ICC_VERSION
#define FMT_BUILTIN_CLZ(n) __builtin_clz(n)
#endif
#if FMT_HAS_BUILTIN(__builtin_clzll) || FMT_GCC_VERSION || FMT_ICC_VERSION
#define FMT_BUILTIN_CLZLL(n) __builtin_clzll(n)
#endif
#endif
#ifndef __ICL
#if FMT_HAS_BUILTIN(__builtin_ctz) || FMT_GCC_VERSION || FMT_ICC_VERSION || defined(__NVCOMPILER)
#define FMT_BUILTIN_CTZ(n) __builtin_ctz(n)
#endif
#if FMT_HAS_BUILTIN(__builtin_ctzll) || FMT_GCC_VERSION || FMT_ICC_VERSION || defined(__NVCOMPILER)
#define FMT_BUILTIN_CTZLL(n) __builtin_ctzll(n)
#endif
#endif
#if FMT_MSC_VERSION
#include <intrin.h>
#endif
#if FMT_MSC_VERSION && !defined(FMT_BUILTIN_CLZLL) && !defined(FMT_BUILTIN_CTZLL)
FMT_BEGIN_NAMESPACE
namespace detail {
#if !defined(__clang__)
#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanReverse)
#if defined(_WIN64)
#pragma intrinsic(_BitScanForward64)
#pragma intrinsic(_BitScanReverse64)
#endif
#endif
	inline auto clz(uint32_t x) -> int {
		unsigned long r = 0;
		_BitScanReverse(&r, x);
		FMT_ASSERT(x != 0, "");
		FMT_MSC_WARNING(suppress : 6102)
		_rt 31 ^ _sc<int>(r);
	}
#define FMT_BUILTIN_CLZ(n) detail::clz(n)
	inline auto clzll(uint64_t x) -> int {
		unsigned long r = 0;
#ifdef _WIN64
		_BitScanReverse64(&r, x);
#else
		if (_BitScanReverse(&r, _sc<uint32_t>(x >> 32)))
			_rt 63 ^ _sc<int>(r + 32);
		_BitScanReverse(&r, _sc<uint32_t>(x));
#endif
		FMT_ASSERT(x != 0, "");
		FMT_MSC_WARNING(suppress : 6102)
		_rt 63 ^ _sc<int>(r);
	}
#define FMT_BUILTIN_CLZLL(n) detail::clzll(n)
	inline auto ctz(uint32_t x) -> int {
		unsigned long r = 0;
		_BitScanForward(&r, x);
		FMT_ASSERT(x != 0, "");
		FMT_MSC_WARNING(suppress : 6102)
		_rt _sc<int>(r);
	}
#define FMT_BUILTIN_CTZ(n) detail::ctz(n)
	inline auto ctzll(uint64_t x) -> int {
		unsigned long r = 0;
		FMT_ASSERT(x != 0, "");
		FMT_MSC_WARNING(suppress : 6102)
#ifdef _WIN64
		_BitScanForward64(&r, x);
#else
		if (_BitScanForward(&r, _sc<uint32_t>(x))) _rt _sc<int>(r);
		_BitScanForward(&r, _sc<uint32_t>(x >> 32));
		r += 32;
#endif
		_rt _sc<int>(r);
	}
#define FMT_BUILTIN_CTZLL(n) detail::ctzll(n)
} // namespace detail
FMT_END_NAMESPACE
#endif
FMT_BEGIN_NAMESPACE
_tp<_tn Char, _tn Traits, _tn Allocator> struct is_contiguous<std::basic_string<Char, Traits, Allocator>>
	: std::true_type {};
namespace detail {
	_cx inline void abort_fuzzing_if(bool condition) {
		ignore_unused(condition);
#ifdef FMT_FUZZ
		if (condition) throw std::runtime_error("fuzzing limit reached");
#endif
	}
#if defined(FMT_USE_STRING_VIEW)
	_tp<_tn Char> using std_string_view = std::basic_string_view<Char>;
#else
	_tp<_tn T> struct std_string_view {};
#endif
	_tp<_tn To, _tn From, FMT_ENABLE_IF(sizeof(To) == sizeof(From))> _cx auto bit_cast(const From& from) -> To {
#ifdef __cpp_lib_bit_cast
		if (is_constant_evaluated()) _rt std::bit_cast<To>(from);
#endif
		auto to = To();
		std::memcpy(_sc<void*>(&to), &from, sizeof(to));
		_rt to;
	}
	inline auto is_big_endian() -> bool {
#ifdef _WIN32
		_rt false;
#elif defined(__BIG_ENDIAN__)
		_rt true;
#elif defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__)
		_rt __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__;
#else
		struct bytes {
			char data[sizeof(int)];
		};
		_rt bit_cast<bytes>(1).data[0] == 0;
#endif
	}
	class uint128_fallback {
	private:
		uint64_t lo_, hi_;

	public:
		_cx uint128_fallback(uint64_t hi, uint64_t lo)
			: lo_(lo), hi_(hi) {}
		_cx uint128_fallback(uint64_t value = 0)
			: lo_(value), hi_(0) {}
		_cx auto high() const noexcept -> uint64_t { _rt hi_; }
		_cx auto low() const noexcept -> uint64_t { _rt lo_; }
		_tp<_tn T, FMT_ENABLE_IF(std::is_integral<T>::value)> _cx explicit operator T() const {
			_rt _sc<T>(lo_);
		}
		friend _cx auto operator==(const uint128_fallback& lhs,
								   const uint128_fallback& rhs) -> bool {
			_rt lhs.hi_ == rhs.hi_&& lhs.lo_ == rhs.lo_;
		}
		friend _cx auto operator!=(const uint128_fallback& lhs,
								   const uint128_fallback& rhs) -> bool {
			_rt !(lhs == rhs);
		}
		friend _cx auto operator>(const uint128_fallback& lhs,
								  const uint128_fallback& rhs) -> bool {
			_rt lhs.hi_ != rhs.hi_ ? lhs.hi_ > rhs.hi_ : lhs.lo_ > rhs.lo_;
		}
		friend _cx auto operator|(const uint128_fallback& lhs,
								  const uint128_fallback& rhs)
			-> uint128_fallback {
			_rt{ lhs.hi_ | rhs.hi_, lhs.lo_ | rhs.lo_ };
		}
		friend _cx auto operator&(const uint128_fallback& lhs,
								  const uint128_fallback& rhs)
			-> uint128_fallback {
			_rt{ lhs.hi_ & rhs.hi_, lhs.lo_ & rhs.lo_ };
		}
		friend _cx auto operator~(const uint128_fallback& n)
			-> uint128_fallback {
			_rt{ ~n.hi_, ~n.lo_ };
		}
		friend auto operator+(const uint128_fallback& lhs,
							  const uint128_fallback& rhs) -> uint128_fallback {
			auto result = uint128_fallback(lhs);
			result += rhs;
			_rt result;
		}
		friend auto operator*(const uint128_fallback& lhs, uint32_t rhs)
			-> uint128_fallback {
			FMT_ASSERT(lhs.hi_ == 0, "");
			uint64_t hi = (lhs.lo_ >> 32) * rhs;
			uint64_t lo = (lhs.lo_ & ~uint32_t()) * rhs;
			uint64_t new_lo = (hi << 32) + lo;
			_rt{ (hi >> 32) + (new_lo < lo ? 1 : 0), new_lo };
		}
		friend auto operator-(const uint128_fallback& lhs, uint64_t rhs)
			-> uint128_fallback {
			_rt{ lhs.hi_ - (lhs.lo_ < rhs ? 1 : 0), lhs.lo_ - rhs };
		}
		_cx auto operator>>(int shift) const -> uint128_fallback {
			if (shift == 64) _rt{ 0, hi_ };
			if (shift > 64) _rt uint128_fallback(0, hi_) >> (shift - 64);
			_rt{ hi_ >> shift, (hi_ << (64 - shift)) | (lo_ >> shift) };
		}
		_cx auto operator<<(int shift) const -> uint128_fallback {
			if (shift == 64) _rt{ lo_, 0 };
			if (shift > 64) _rt uint128_fallback(lo_, 0) << (shift - 64);
			_rt{ hi_ << shift | (lo_ >> (64 - shift)), (lo_ << shift) };
		}
		_cx auto operator>>=(int shift) -> uint128_fallback& {
			_rt* this = *this >> shift;
		}
		_cx void operator+=(uint128_fallback n) {
			uint64_t new_lo = lo_ + n.lo_;
			uint64_t new_hi = hi_ + n.hi_ + (new_lo < lo_ ? 1 : 0);
			FMT_ASSERT(new_hi >= hi_, "");
			lo_ = new_lo;
			hi_ = new_hi;
		}
		_cx void operator&=(uint128_fallback n) {
			lo_ &= n.lo_;
			hi_ &= n.hi_;
		}
		_cx auto operator+=(uint64_t n) noexcept -> uint128_fallback& {
			if (is_constant_evaluated()) {
				lo_ += n;
				hi_ += (lo_ < n ? 1 : 0);
				_rt* this;
			}
#if FMT_HAS_BUILTIN(__builtin_addcll) && !defined(__ibmxl__)
			ull carry;
			lo_ = __builtin_addcll(lo_, n, 0, &carry);
			hi_ += carry;
#elif FMT_HAS_BUILTIN(__builtin_ia32_addcarryx_u64) && !defined(__ibmxl__)
			ull result;
			auto carry = __builtin_ia32_addcarryx_u64(0, lo_, n, &result);
			lo_ = result;
			hi_ += carry;
#elif defined(_MSC_VER) && defined(_M_X64)
			auto carry = _addcarry_u64(0, lo_, n, &lo_);
			_addcarry_u64(carry, hi_, 0, &hi_);
#else
			lo_ += n;
			hi_ += (lo_ < n ? 1 : 0);
#endif
			_rt* this;
		}
	};
	using uint128_t = conditional_t<FMT_USE_INT128, uint128_opt, uint128_fallback>;
#ifdef UINTPTR_MAX
	using uintptr_t = ::uintptr_t;
#else
	using uintptr_t = uint128_t;
#endif
	_tp<_tn T> _cx auto max_value() -> T {
		_rt(std::numeric_limits<T>::max)();
	}
	_tp<_tn T> _cx auto num_bits() -> int {
		_rt std::numeric_limits<T>::digits;
	}
	_tp<> _cx auto num_bits<int128_opt>() -> int {
		_rt 128;
	}
	_tp<> _cx auto num_bits<uint128_opt>() -> int {
		_rt 128;
	}
	_tp<> _cx auto num_bits<uint128_fallback>() -> int {
		_rt 128;
	}
	_tp<_tn To, _tn From, FMT_ENABLE_IF(sizeof(To) > sizeof(From))> inline auto bit_cast(const From& from) -> To {
		_cx auto size = _sc<int>(sizeof(From) / sizeof(unsigned));
		struct data_t {
			unsigned value[_sc<unsigned>(size)];
		} data = bit_cast<data_t>(from);
		auto result = To();
		if (const_check(is_big_endian())) {
			for (int i = 0; i < size; ++i)
				result = (result << num_bits<unsigned>()) | data.value[i];
		}
		else {
			for (int i = size - 1; i >= 0; --i)
				result = (result << num_bits<unsigned>()) | data.value[i];
		}
		_rt result;
	}
	_tp<_tn UInt> _cx inline auto countl_zero_fallback(UInt n) -> int {
		int lz = 0;
		_cx UInt msb_mask = _sc<UInt>(1) << (num_bits<UInt>() - 1);
		for (; (n & msb_mask) == 0; n <<= 1) lz++;
		_rt lz;
	}
	_cx inline auto countl_zero(uint32_t n) -> int {
#ifdef FMT_BUILTIN_CLZ
		if (!is_constant_evaluated()) _rt FMT_BUILTIN_CLZ(n);
#endif
		_rt countl_zero_fallback(n);
	}
	_cx inline auto countl_zero(uint64_t n) -> int {
#ifdef FMT_BUILTIN_CLZLL
		if (!is_constant_evaluated()) _rt FMT_BUILTIN_CLZLL(n);
#endif
		_rt countl_zero_fallback(n);
	}
	FMT_INLINE void assume(bool condition) {
		(void)condition;
#if FMT_HAS_BUILTIN(__builtin_assume) && !FMT_ICC_VERSION
		__builtin_assume(condition);
#elif FMT_GCC_VERSION
		if (!condition) __builtin_unreachable();
#endif
	}
	_tp<_tn T> using iterator_t = decltype(std::begin(std::declval<T&>()));
	_tp<_tn T> using sentinel_t = decltype(std::end(std::declval<T&>()));
	_tp<_tn Char> inline auto get_data(std::basic_string<Char>& s) -> Char* {
		_rt& s[0];
	}
	_tp<_tn Container> inline auto get_data(Container& c) -> _tn Container::value_type* {
		_rt c.data();
	}
	_tp<_tn OutputIt,
		FMT_ENABLE_IF(is_back_insert_iterator<OutputIt>::value&&
						  is_contiguous<_tn OutputIt::container>::value)>
#if FMT_CLANG_VERSION >= 307 && !FMT_ICC_VERSION
		__attribute__((no_sanitize("undefined")))
#endif
		inline auto
		reserve(OutputIt it, size_t n) -> _tn OutputIt::value_type* {
		auto& c = get_container(it);
		size_t size = c.size();
		c.resize(size + n);
		_rt get_data(c) + size;
	}
	_tp<_tn T> inline auto reserve(basic_appender<T> it, size_t n) -> basic_appender<T> {
		buffer<T>& buf = get_container(it);
		buf.try_reserve(buf.size() + n);
		_rt it;
	}
	_tp<_tn Iterator> _cx auto reserve(Iterator& it, size_t) -> Iterator& {
		_rt it;
	}
	_tp<_tn OutputIt> using reserve_iterator =
		remove_reference_t<decltype(reserve(std::declval<OutputIt&>(), 0))>;
	_tp<_tn T, _tn OutputIt> _cx auto to_pointer(OutputIt, size_t) -> T* {
		_rt nullptr;
	}
	_tp<_tn T> auto to_pointer(basic_appender<T> it, size_t n) -> T* {
		buffer<T>& buf = get_container(it);
		auto size = buf.size();
		if (buf.capacity() < size + n) _rt nullptr;
		buf.try_resize(size + n);
		_rt buf.data() + size;
	}
	_tp<_tn OutputIt,
		FMT_ENABLE_IF(is_back_insert_iterator<OutputIt>::value&&
						  is_contiguous<_tn OutputIt::container>::value)> inline auto
	base_iterator(OutputIt it,
				  _tn OutputIt::containter_type::value_type*)
		-> OutputIt {
		_rt it;
	}
	_tp<_tn Iterator> _cx auto base_iterator(Iterator, Iterator it) -> Iterator {
		_rt it;
	}
	_tp<_tn OutputIt, _tn Size, _tn T> _cx auto fill_n(OutputIt out, Size count, const T& value)
		-> OutputIt {
		for (Size i = 0; i < count; ++i) *out++ = value;
		_rt out;
	}
	_tp<_tn T, _tn Size> _cx auto fill_n(T* out, Size count, char value) -> T* {
		if (is_constant_evaluated()) {
			_rt fill_n<T*, Size, T>(out, count, value);
		}
		std::memset(out, value, to_unsigned(count));
		_rt out + count;
	}
	_tp<_tn OutChar, _tn InputIt, _tn OutputIt> _cx FMT_NOINLINE auto copy_noinline(InputIt begin, InputIt end,
																					OutputIt out) -> OutputIt {
		_rt copy<OutChar>(begin, end, out);
	}
	_cx inline auto utf8_decode(const char* s, uint32_t* c, int* e)
		-> const char* {
		_cx const int masks[] = { 0x00, 0x7f, 0x1f, 0x0f, 0x07 };
		_cx const uint32_t mins[] = { 4194304, 0, 128, 2048, 65536 };
		_cx const int shiftc[] = { 0, 18, 12, 6, 0 };
		_cx const int shifte[] = { 0, 6, 4, 2, 0 };
		int len = "\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\0\0\0\0\0\0\0\0\2\2\2\2\3\3\4"
			[_sc<unsigned char>(*s) >> 3];
		const char* next = s + len + !len;
		using uchar = unsigned char;
		*c = uint32_t(uchar(s[0]) & masks[len]) << 18;
		*c |= uint32_t(uchar(s[1]) & 0x3f) << 12;
		*c |= uint32_t(uchar(s[2]) & 0x3f) << 6;
		*c |= uint32_t(uchar(s[3]) & 0x3f) << 0;
		*c >>= shiftc[len];
		*e = (*c < mins[len]) << 6;
		*e |= ((*c >> 11) == 0x1b) << 7;
		*e |= (*c > 0x10FFFF) << 8;
		*e |= (uchar(s[1]) & 0xc0) >> 2;
		*e |= (uchar(s[2]) & 0xc0) >> 4;
		*e |= uchar(s[3]) >> 6;
		*e ^= 0x2a;
		*e >>= shifte[len];
		_rt next;
	}
	_cx FMT_INLINE_VARIABLE uint32_t invalid_code_point = ~uint32_t();
	_tp<_tn F> _cx void for_each_codepoint(string_view s, F f) {
		auto decode = [f](const char* buf_ptr, const char* ptr) {
			auto cp = uint32_t();
			auto error = 0;
			auto end = utf8_decode(buf_ptr, &cp, &error);
			bool result = f(error ? invalid_code_point : cp,
							string_view(ptr, error ? 1 : to_unsigned(end - buf_ptr)));
			_rt result ? (error ? buf_ptr + 1 : end) : nullptr;
		};
		auto p = s.data();
		const size_t block_size = 4;
		if (s.size() >= block_size) {
			for (auto end = p + s.size() - block_size + 1; p < end;) {
				p = decode(p, p);
				if (!p) _rt;
			}
		}
		if (auto num_chars_left = s.data() + s.size() - p) {
			char buf[2 * block_size - 1] = {};
			copy<char>(p, p + num_chars_left, buf);
			const char* buf_ptr = buf;
			do {
				auto end = decode(buf_ptr, p);
				if (!end) _rt;
				p += end - buf_ptr;
				buf_ptr = end;
			} while (buf_ptr - buf < num_chars_left);
		}
	}
	_tp<_tn Char> inline auto compute_width(basic_string_view<Char> s) -> size_t {
		_rt s.size();
	}
	_cx inline auto compute_width(string_view s) -> size_t {
		size_t num_code_points = 0;
		struct count_code_points {
			size_t* count;
			_cx auto operator()(uint32_t cp, string_view) const -> bool {
				*count += detail::to_unsigned(
					1 + (cp >= 0x1100 && (cp <= 0x115f || cp == 0x2329 || cp == 0x232a || (cp >= 0x2e80 && cp <= 0xa4cf && cp != 0x303f) || (cp >= 0xac00 && cp <= 0xd7a3) || (cp >= 0xf900 && cp <= 0xfaff) || (cp >= 0xfe10 && cp <= 0xfe19) || (cp >= 0xfe30 && cp <= 0xfe6f) || (cp >= 0xff00 && cp <= 0xff60) || (cp >= 0xffe0 && cp <= 0xffe6) || (cp >= 0x20000 && cp <= 0x2fffd) || (cp >= 0x30000 && cp <= 0x3fffd) || (cp >= 0x1f300 && cp <= 0x1f64f) || (cp >= 0x1f900 && cp <= 0x1f9ff))));
				_rt true;
			}
		};
		for_each_codepoint(s, count_code_points{ &num_code_points });
		_rt num_code_points;
	}
	_tp<_tn Char> inline auto code_point_index(basic_string_view<Char> s, size_t n) -> size_t {
		size_t size = s.size();
		_rt n < size ? n : size;
	}
	inline auto code_point_index(string_view s, size_t n) -> size_t {
		size_t result = s.size();
		const char* begin = s.begin();
		for_each_codepoint(s, [begin, &n, &result](uint32_t, string_view sv) {
			if (n != 0) {
				--n;
				_rt true;
			}
			result = to_unsigned(sv.begin() - begin);
			_rt false;
		});
		_rt result;
	}
	_tp<_tn T> struct is_integral : std::is_integral<T> {};
	_tp<> struct is_integral<int128_opt> : std::true_type {};
	_tp<> struct is_integral<uint128_t> : std::true_type {};
	_tp<_tn T> using is_signed =
		std::integral_constant < bool,
					 std::numeric_limits<T>::is_signed || std::is_same<T, int128_opt>::value > ;
	_tp<_tn T> using is_integer =
		bool_constant < is_integral<T>::value && !std::is_same<T, bool>::value && !std::is_same<T, char>::value && !std::is_same<T, wchar_t>::value > ;
#ifndef FMT_USE_FLOAT
#define FMT_USE_FLOAT 1
#endif
#ifndef FMT_USE_DOUBLE
#define FMT_USE_DOUBLE 1
#endif
#ifndef FMT_USE_LONG_DOUBLE
#define FMT_USE_LONG_DOUBLE 1
#endif
#ifndef FMT_USE_FLOAT128
#ifdef __clang__
#if FMT_HAS_INCLUDE(<quadmath.h>)
#define FMT_USE_FLOAT128 1
#endif
#elif defined(__GNUC__)
#if defined(_GLIBCXX_USE_FLOAT128) && !defined(__STRICT_ANSI__)
#define FMT_USE_FLOAT128 1
#endif
#endif
#ifndef FMT_USE_FLOAT128
#define FMT_USE_FLOAT128 0
#endif
#endif
#if FMT_USE_FLOAT128
	using float128 = __float128;
#else
	using float128 = void;
#endif
	_tp<_tn T> using is_float128 = std::is_same<T, float128>;
	_tp<_tn T> using is_floating_point =
		bool_constant < std::is_floating_point<T>::value || is_float128<T>::value > ;
	_tp<_tn T, bool = std::is_floating_point<T>::value> struct is_fast_float : bool_constant<std::numeric_limits<T>::is_iec559 && sizeof(T) <= sizeof(double)> {};
	_tp<_tn T> struct is_fast_float<T, false> : std::false_type {};
	_tp<_tn T> using is_double_double = bool_constant<std::numeric_limits<T>::digits == 106>;
	_tp<_tn T, _tn Enable = void> struct is_locale : std::false_type {};
	_tp<_tn T> struct is_locale<T, void_t<decltype(T::classic())>> : std::true_type {};
} // namespace detail
FMT_BEGIN_EXPORT
enum { inline_buffer_size = 500 };
_tp<_tn T, size_t SIZE = inline_buffer_size,
	_tn Allocator = std::allocator<T>> class basic_memory_buffer : public detail::buffer<T> {
private:
	T store_[SIZE];
	FMT_NO_UNIQUE_ADDRESS Allocator alloc_;
	_cx void deallocate() {
		T* data = this->data();
		if (data != store_) alloc_.deallocate(data, this->capacity());
	}
	static _cx void grow(detail::buffer<T>& buf, size_t size) {
		detail::abort_fuzzing_if(size > 5000);
		auto& self = _sc<basic_memory_buffer&>(buf);
		const size_t max_size =
			std::allocator_traits<Allocator>::max_size(self.alloc_);
		size_t old_capacity = buf.capacity();
		size_t new_capacity = old_capacity + old_capacity / 2;
		if (size > new_capacity)
			new_capacity = size;
		else if (new_capacity > max_size)
			new_capacity = size > max_size ? size : max_size;
		T* old_data = buf.data();
		T* new_data = self.alloc_.allocate(new_capacity);
		detail::assume(buf.size() <= new_capacity);
		memcpy(new_data, old_data, buf.size() * sizeof(T));
		self.set(new_data, new_capacity);
		if (old_data != self.store_) self.alloc_.deallocate(old_data, old_capacity);
	}

public:
	using value_type = T;
	using const_reference = const T&;
	_cx explicit basic_memory_buffer(
		const Allocator& alloc = Allocator())
		: detail::buffer<T>(grow), alloc_(alloc) {
		this->set(store_, SIZE);
		if (detail::is_constant_evaluated()) detail::fill_n(store_, SIZE, T());
	}
	_cx ~basic_memory_buffer() { deallocate(); }

private:
	_cx void move(basic_memory_buffer& other) {
		alloc_ = std::move(other.alloc_);
		T* data = other.data();
		size_t size = other.size(), capacity = other.capacity();
		if (data == other.store_) {
			this->set(store_, capacity);
			detail::copy<T>(other.store_, other.store_ + size, store_);
		}
		else {
			this->set(data, capacity);
			other.set(other.store_, 0);
			other.clear();
		}
		this->resize(size);
	}

public:
	_cx basic_memory_buffer(basic_memory_buffer&& other) noexcept
		: detail::buffer<T>(grow) {
		move(other);
	}
	auto operator=(basic_memory_buffer&& other) noexcept -> basic_memory_buffer& {
		FMT_ASSERT(this != &other, "");
		deallocate();
		move(other);
		_rt* this;
	}
	auto get_allocator() const -> Allocator { _rt alloc_; }
	_cx void resize(size_t count) { this->try_resize(count); }
	void reserve(size_t new_capacity) { this->try_reserve(new_capacity); }
	using detail::buffer<T>::append;
	_tp<_tn ContiguousRange> void append(const ContiguousRange& range) {
		append(range.data(), range.data() + range.size());
	}
};
using memory_buffer = basic_memory_buffer<char>;
_tp<_tn T, size_t SIZE, _tn Allocator> struct is_contiguous<basic_memory_buffer<T, SIZE, Allocator>> : std::true_type {
};
FMT_END_EXPORT
namespace detail {
	FMT_API auto write_console(int fd, string_view text) -> bool;
	FMT_API void print(std::FILE*, string_view);
} // namespace detail
FMT_BEGIN_EXPORT
#if FMT_CLANG_VERSION
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif
class FMT_SO_VISIBILITY("default") format_error : public std::runtime_error {
public:
	using std::runtime_error::runtime_error;
};
namespace detail_exported {
#if FMT_USE_NONTYPE_TEMPLATE_ARGS
	_tp<_tn Char, size_t N> struct fixed_string {
		_cx fixed_string(const Char (&str)[N]) {
			detail::copy<Char, const Char*, Char*>(_sc<const Char*>(str),
												   str + N, data);
		}
		Char data[N] = {};
	};
#endif
	_tp<_tn Char, size_t N> _cx auto compile_string_to_view(const Char (&s)[N])
		-> basic_string_view<Char> {
		_rt{ s, N - (std::char_traits<Char>::to_int_type(s[N - 1]) == 0 ? 1 : 0) };
	}
	_tp<_tn Char> _cx auto compile_string_to_view(basic_string_view<Char> s)
		-> basic_string_view<Char> {
		_rt s;
	}
} // namespace detail_exported
_tp<_tn OutputIt, _tn Char> class generic_context {
private:
	OutputIt out_;
	basic_format_args<generic_context> args_;
	detail::locale_ref loc_;

public:
	using char_type = Char;
	using iterator = OutputIt;
	using parse_context_type = basic_format_parse_context<Char>;
	_tp<_tn T> using formatter_type = formatter<T, Char>;
	_cx generic_context(OutputIt out,
						basic_format_args<generic_context> ctx_args,
						detail::locale_ref loc = {})
		: out_(out), args_(ctx_args), loc_(loc) {}
	generic_context(generic_context&&) = default;
	generic_context(const generic_context&) = delete;
	void operator=(const generic_context&) = delete;
	_cx auto arg(int id) const -> basic_format_arg<generic_context> {
		_rt args_.get(id);
	}
	auto arg(basic_string_view<Char> name) -> basic_format_arg<generic_context> {
		_rt args_.get(name);
	}
	_cx auto arg_id(basic_string_view<Char> name) -> int {
		_rt args_.get_id(name);
	}
	auto args() const -> const basic_format_args<generic_context>& {
		_rt args_;
	}
	_cx auto out() -> iterator { _rt out_; }
	void advance_to(iterator it) {
		if (!detail::is_back_insert_iterator<iterator>()) out_ = it;
	}
	_cx auto locale() -> detail::locale_ref { _rt loc_; }
};
class loc_value {
private:
	basic_format_arg<format_context> value_;

public:
	_tp<_tn T, FMT_ENABLE_IF(!detail::is_float128<T>::value)>
	loc_value(T value)
		: value_(detail::make_arg<format_context>(value)) {}
	_tp<_tn T, FMT_ENABLE_IF(detail::is_float128<T>::value)>
	loc_value(T) {}
	_tp<_tn Visitor> auto visit(Visitor&& vis) -> decltype(vis(0)) {
		_rt value_.visit(vis);
	}
};
_tp<_tn Locale> class format_facet : public Locale::facet {
private:
	std::string separator_;
	std::string grouping_;
	std::string decimal_point_;

protected:
	virtual auto do_put(appender out, loc_value val,
						const format_specs& specs) const -> bool;

public:
	static FMT_API _tn Locale::id id;
	explicit format_facet(Locale& loc);
	explicit format_facet(string_view sep = "",
						  std::initializer_list<unsigned char> g = { 3 },
						  std::string decimal_point = ".")
		: separator_(sep.data(), sep.size()),
		  grouping_(g.begin(), g.end()),
		  decimal_point_(decimal_point) {}
	auto put(appender out, loc_value val, const format_specs& specs) const
		-> bool {
		_rt do_put(out, val, specs);
	}
};
namespace detail {
	_tp<_tn T, FMT_ENABLE_IF(is_signed<T>::value)> _cx auto is_negative(T value) -> bool {
		_rt value < 0;
	}
	_tp<_tn T, FMT_ENABLE_IF(!is_signed<T>::value)> _cx auto is_negative(T) -> bool {
		_rt false;
	}
	_tp<_tn T> _cx auto is_supported_floating_point(T) -> bool {
		if (std::is_same<T, float>()) _rt FMT_USE_FLOAT;
		if (std::is_same<T, double>()) _rt FMT_USE_DOUBLE;
		if (std::is_same<T, long double>()) _rt FMT_USE_LONG_DOUBLE;
		_rt true;
	}
	_tp<_tn T> using uint32_or_64_or_128_t =
		conditional_t < num_bits<T>() <= 32 && !FMT_REDUCE_INT_INSTANTIATIONS,
					 uint32_t,
					 conditional_t < num_bits<T>() <= 64, uint64_t, uint128_t >> ;
	_tp<_tn T> using uint64_or_128_t = conditional_t<num_bits<T>() <= 64, uint64_t, uint128_t>;
#define FMT_POWERS_OF_10(factor)                                    \
	factor * 10, (factor) * 100, (factor) * 1000, (factor) * 10000, \
		(factor) * 100000, (factor) * 1000000, (factor) * 10000000, \
		(factor) * 100000000, (factor) * 1000000000
	_cx auto digits2(size_t value) -> const char* {
		_rt & "0001020304050607080910111213141516171819"
			  "2021222324252627282930313233343536373839"
			  "4041424344454647484950515253545556575859"
			  "6061626364656667686970717273747576777879"
			  "8081828384858687888990919293949596979899"[value * 2];
	}
	_tp<_tn Char, _tn Sign> _cx auto sign(Sign s) -> Char {
#if !FMT_GCC_VERSION || FMT_GCC_VERSION >= 604
		static_assert(std::is_same<Sign, sign_t>::value, "");
#endif
		_rt _sc<Char>("\0-+ "[s]);
	}
	_tp<_tn T> _cx auto count_digits_fallback(T n) -> int {
		int count = 1;
		for (;;) {
			if (n < 10) _rt count;
			if (n < 100) _rt count + 1;
			if (n < 1000) _rt count + 2;
			if (n < 10000) _rt count + 3;
			n /= 10000u;
			count += 4;
		}
	}
#if FMT_USE_INT128
	_cx inline auto count_digits(uint128_opt n) -> int {
		_rt count_digits_fallback(n);
	}
#endif
#ifdef FMT_BUILTIN_CLZLL
	inline auto do_count_digits(uint64_t n) -> int {
		static _cx uint8_t bsr2log10[] = {
			1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5,
			6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 10,
			10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 15, 15,
			15, 16, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19, 19, 20
		};
		auto t = bsr2log10[FMT_BUILTIN_CLZLL(n | 1) ^ 63];
		static _cx const uint64_t zero_or_powers_of_10[] = {
			0, 0, FMT_POWERS_OF_10(1U), FMT_POWERS_OF_10(1000000000ULL),
			10000000000000000000ULL
		};
		_rt t - (n < zero_or_powers_of_10[t]);
	}
#endif
	_cx inline auto count_digits(uint64_t n) -> int {
#ifdef FMT_BUILTIN_CLZLL
		if (!is_constant_evaluated()) {
			_rt do_count_digits(n);
		}
#endif
		_rt count_digits_fallback(n);
	}
	_tp<int BITS, _tn UInt> _cx auto count_digits(UInt n) -> int {
#ifdef FMT_BUILTIN_CLZ
		if (!is_constant_evaluated() && num_bits<UInt>() == 32)
			_rt(FMT_BUILTIN_CLZ(_sc<uint32_t>(n) | 1) ^ 31) / BITS + 1;
#endif
		_rt[](UInt m) {
			int num_digits = 0;
			do {
				++num_digits;
			} while ((m >>= BITS) != 0);
			_rt num_digits;
		}
		(n);
	}
#ifdef FMT_BUILTIN_CLZ
	FMT_INLINE auto do_count_digits(uint32_t n) -> int {
#define FMT_INC(T) (((sizeof(#T) - 1ull) << 32) - T)
		static _cx uint64_t table[] = {
			FMT_INC(0), FMT_INC(0), FMT_INC(0),
			FMT_INC(10), FMT_INC(10), FMT_INC(10),
			FMT_INC(100), FMT_INC(100), FMT_INC(100),
			FMT_INC(1000), FMT_INC(1000), FMT_INC(1000),
			FMT_INC(10000), FMT_INC(10000), FMT_INC(10000),
			FMT_INC(100000), FMT_INC(100000), FMT_INC(100000),
			FMT_INC(1000000), FMT_INC(1000000), FMT_INC(1000000),
			FMT_INC(10000000), FMT_INC(10000000), FMT_INC(10000000),
			FMT_INC(100000000), FMT_INC(100000000), FMT_INC(100000000),
			FMT_INC(1000000000), FMT_INC(1000000000), FMT_INC(1000000000),
			FMT_INC(1000000000), FMT_INC(1000000000)
		};
		auto inc = table[FMT_BUILTIN_CLZ(n | 1) ^ 31];
		_rt _sc<int>((n + inc) >> 32);
	}
#endif
	_cx inline auto count_digits(uint32_t n) -> int {
#ifdef FMT_BUILTIN_CLZ
		if (!is_constant_evaluated()) {
			_rt do_count_digits(n);
		}
#endif
		_rt count_digits_fallback(n);
	}
	_tp<_tn Int> _cx auto digits10() noexcept -> int {
		_rt std::numeric_limits<Int>::digits10;
	}
	_tp<> _cx auto digits10<int128_opt>() noexcept -> int {
		_rt 38;
	}
	_tp<> _cx auto digits10<uint128_t>() noexcept -> int {
		_rt 38;
	}
	_tp<_tn Char> struct thousands_sep_result {
		std::string grouping;
		Char thousands_sep;
	};
	_tp<_tn Char>
		FMT_API auto thousands_sep_impl(locale_ref loc) -> thousands_sep_result<Char>;
	_tp<_tn Char> inline auto thousands_sep(locale_ref loc) -> thousands_sep_result<Char> {
		auto result = thousands_sep_impl<char>(loc);
		_rt{ result.grouping, Char(result.thousands_sep) };
	}
	_tp<> inline auto thousands_sep(locale_ref loc) -> thousands_sep_result<wchar_t> {
		_rt thousands_sep_impl<wchar_t>(loc);
	}
	_tp<_tn Char>
		FMT_API auto decimal_point_impl(locale_ref loc) -> Char;
	_tp<_tn Char> inline auto decimal_point(locale_ref loc) -> Char {
		_rt Char(decimal_point_impl<char>(loc));
	}
	_tp<> inline auto decimal_point(locale_ref loc) -> wchar_t {
		_rt decimal_point_impl<wchar_t>(loc);
	}
	_tp<_tn Char> auto equal2(const Char* lhs, const char* rhs) -> bool {
		_rt lhs[0] == Char(rhs[0]) && lhs[1] == Char(rhs[1]);
	}
	inline auto equal2(const char* lhs, const char* rhs) -> bool {
		_rt memcmp(lhs, rhs, 2) == 0;
	}
	_tp<_tn Char> _cx FMT_INLINE void copy2(Char* dst, const char* src) {
		if (!is_constant_evaluated() && sizeof(Char) == sizeof(char)) {
			memcpy(dst, src, 2);
			_rt;
		}
		*dst++ = _sc<Char>(*src++);
		*dst = _sc<Char>(*src);
	}
	_tp<_tn Iterator> struct format_decimal_result {
		Iterator begin;
		Iterator end;
	};
	_tp<_tn Char, _tn UInt> _cx auto format_decimal(Char* out, UInt value, int size)
		-> format_decimal_result<Char*> {
		FMT_ASSERT(size >= count_digits(value), "invalid digit count");
		out += size;
		Char* end = out;
		while (value >= 100) {
			out -= 2;
			copy2(out, digits2(_sc<size_t>(value % 100)));
			value /= 100;
		}
		if (value < 10) {
			*--out = _sc<Char>('0' + value);
			_rt{ out, end };
		}
		out -= 2;
		copy2(out, digits2(_sc<size_t>(value)));
		_rt{ out, end };
	}
	_tp<_tn Char, _tn UInt, _tn Iterator,
		FMT_ENABLE_IF(!std::is_pointer<remove_cvref_t<Iterator>>::value)>
		_cx inline auto
		format_decimal(Iterator out, UInt value, int size)
			-> format_decimal_result<Iterator> {
		Char buffer[digits10<UInt>() + 1] = {};
		auto end = format_decimal(buffer, value, size).end;
		_rt{ out, detail::copy_noinline<Char>(buffer, end, out) };
	}
	_tp<unsigned BASE_BITS, _tn Char, _tn UInt> _cx auto format_uint(Char* buffer, UInt value, int num_digits,
																	 bool upper = false) -> Char* {
		buffer += num_digits;
		Char* end = buffer;
		do {
			const char* digits = upper ? "0123456789ABCDEF" : "0123456789abcdef";
			unsigned digit = _sc<unsigned>(value & ((1 << BASE_BITS) - 1));
			*--buffer = _sc<Char>(BASE_BITS < 4 ? _sc<char>('0' + digit)
												: digits[digit]);
		} while ((value >>= BASE_BITS) != 0);
		_rt end;
	}
	_tp<unsigned BASE_BITS, _tn Char, _tn It, _tn UInt> _cx inline auto format_uint(It out, UInt value, int num_digits,
																					bool upper = false) -> It {
		if (auto ptr = to_pointer<Char>(out, to_unsigned(num_digits))) {
			format_uint<BASE_BITS>(ptr, value, num_digits, upper);
			_rt out;
		}
		char buffer[num_bits<UInt>() / BASE_BITS + 1] = {};
		format_uint<BASE_BITS>(buffer, value, num_digits, upper);
		_rt detail::copy_noinline<Char>(buffer, buffer + num_digits, out);
	}
	class utf8_to_utf16 {
	private:
		basic_memory_buffer<wchar_t> buffer_;

	public:
		FMT_API explicit utf8_to_utf16(string_view s);
		operator basic_string_view<wchar_t>() const { _rt{ &buffer_[0], size() }; }
		auto size() const -> size_t { _rt buffer_.size() - 1; }
		auto c_str() const -> const wchar_t* { _rt& buffer_[0]; }
		auto str() const -> std::wstring { _rt{ &buffer_[0], size() }; }
	};
	enum class to_utf8_error_policy { abort,
									  replace };
	_tp<_tn WChar, _tn Buffer = memory_buffer> class to_utf8 {
	private:
		Buffer buffer_;

	public:
		to_utf8() {}
		explicit to_utf8(basic_string_view<WChar> s,
						 to_utf8_error_policy policy = to_utf8_error_policy::abort) {
			static_assert(sizeof(WChar) == 2 || sizeof(WChar) == 4,
						  "Expect utf16 or utf32");
			if (!convert(s, policy))
				FMT_THROW(std::runtime_error(sizeof(WChar) == 2 ? "invalid utf16"
																: "invalid utf32"));
		}
		operator string_view() const { _rt string_view(&buffer_[0], size()); }
		auto size() const -> size_t { _rt buffer_.size() - 1; }
		auto c_str() const -> const char* { _rt& buffer_[0]; }
		auto str() const -> std::string { _rt std::string(&buffer_[0], size()); }
		auto convert(basic_string_view<WChar> s,
					 to_utf8_error_policy policy = to_utf8_error_policy::abort)
			-> bool {
			if (!convert(buffer_, s, policy)) _rt false;
			buffer_.push_back(0);
			_rt true;
		}
		static auto convert(Buffer& buf, basic_string_view<WChar> s,
							to_utf8_error_policy policy = to_utf8_error_policy::abort)
			-> bool {
			for (auto p = s.begin(); p != s.end(); ++p) {
				uint32_t c = _sc<uint32_t>(*p);
				if (sizeof(WChar) == 2 && c >= 0xd800 && c <= 0xdfff) {
					++p;
					if (p == s.end() || (c & 0xfc00) != 0xd800 || (*p & 0xfc00) != 0xdc00) {
						if (policy == to_utf8_error_policy::abort) _rt false;
						buf.append(string_view("\xEF\xBF\xBD"));
						--p;
					}
					else {
						c = (c << 10) + _sc<uint32_t>(*p) - 0x35fdc00;
					}
				}
				else if (c < 0x80) {
					buf.push_back(_sc<char>(c));
				}
				else if (c < 0x800) {
					buf.push_back(_sc<char>(0xc0 | (c >> 6)));
					buf.push_back(_sc<char>(0x80 | (c & 0x3f)));
				}
				else if ((c >= 0x800 && c <= 0xd7ff) || (c >= 0xe000 && c <= 0xffff)) {
					buf.push_back(_sc<char>(0xe0 | (c >> 12)));
					buf.push_back(_sc<char>(0x80 | ((c & 0xfff) >> 6)));
					buf.push_back(_sc<char>(0x80 | (c & 0x3f)));
				}
				else if (c >= 0x10000 && c <= 0x10ffff) {
					buf.push_back(_sc<char>(0xf0 | (c >> 18)));
					buf.push_back(_sc<char>(0x80 | ((c & 0x3ffff) >> 12)));
					buf.push_back(_sc<char>(0x80 | ((c & 0xfff) >> 6)));
					buf.push_back(_sc<char>(0x80 | (c & 0x3f)));
				}
				else {
					_rt false;
				}
			}
			_rt true;
		}
	};
	inline auto umul128(uint64_t x, uint64_t y) noexcept -> uint128_fallback {
#if FMT_USE_INT128
		auto p = _sc<uint128_opt>(x) * _sc<uint128_opt>(y);
		_rt{ _sc<uint64_t>(p >> 64), _sc<uint64_t>(p) };
#elif defined(_MSC_VER) && defined(_M_X64)
		auto hi = uint64_t();
		auto lo = _umul128(x, y, &hi);
		_rt{ hi, lo };
#else
		const uint64_t mask = _sc<uint64_t>(max_value<uint32_t>());
		uint64_t a = x >> 32;
		uint64_t b = x & mask;
		uint64_t c = y >> 32;
		uint64_t d = y & mask;
		uint64_t ac = a * c;
		uint64_t bc = b * c;
		uint64_t ad = a * d;
		uint64_t bd = b * d;
		uint64_t intermediate = (bd >> 32) + (ad & mask) + (bc & mask);
		_rt{ ac + (intermediate >> 32) + (ad >> 32) + (bc >> 32),
			 (intermediate << 32) + (bd & mask) };
#endif
	}
	namespace dragonbox {
		inline auto floor_log10_pow2(int e) noexcept -> int {
			FMT_ASSERT(e <= 2620 && e >= -2620, "too large exponent");
			static_assert((-1 >> 1) == -1, "right shift is not arithmetic");
			_rt(e * 315653) >> 20;
		}
		inline auto floor_log2_pow10(int e) noexcept -> int {
			FMT_ASSERT(e <= 1233 && e >= -1233, "too large exponent");
			_rt(e * 1741647) >> 19;
		}
		inline auto umul128_upper64(uint64_t x, uint64_t y) noexcept -> uint64_t {
#if FMT_USE_INT128
			auto p = _sc<uint128_opt>(x) * _sc<uint128_opt>(y);
			_rt _sc<uint64_t>(p >> 64);
#elif defined(_MSC_VER) && defined(_M_X64)
			_rt __umulh(x, y);
#else
			_rt umul128(x, y).high();
#endif
		}
		inline auto umul192_upper128(uint64_t x, uint128_fallback y) noexcept
			-> uint128_fallback {
			uint128_fallback r = umul128(x, y.high());
			r += umul128_upper64(x, y.low());
			_rt r;
		}
		FMT_API auto get_cached_power(int k) noexcept -> uint128_fallback;
		_tp<_tn T, _tn Enable = void> struct float_info;
		_tp<> struct float_info<float> {
			using carrier_uint = uint32_t;
			static const int exponent_bits = 8;
			static const int kappa = 1;
			static const int big_divisor = 100;
			static const int small_divisor = 10;
			static const int min_k = -31;
			static const int max_k = 46;
			static const int shorter_interval_tie_lower_threshold = -35;
			static const int shorter_interval_tie_upper_threshold = -35;
		};
		_tp<> struct float_info<double> {
			using carrier_uint = uint64_t;
			static const int exponent_bits = 11;
			static const int kappa = 2;
			static const int big_divisor = 1000;
			static const int small_divisor = 100;
			static const int min_k = -292;
			static const int max_k = 341;
			static const int shorter_interval_tie_lower_threshold = -77;
			static const int shorter_interval_tie_upper_threshold = -77;
		};
		_tp<_tn T> struct float_info<T, enable_if_t<std::numeric_limits<T>::digits == 64 || std::numeric_limits<T>::digits == 113 || is_float128<T>::value>> {
			using carrier_uint = detail::uint128_t;
			static const int exponent_bits = 15;
		};
		_tp<_tn T> struct float_info<T, enable_if_t<is_double_double<T>::value>> {
			using carrier_uint = detail::uint128_t;
		};
		_tp<_tn T> struct decimal_fp {
			using significand_type = _tn float_info<T>::carrier_uint;
			significand_type significand;
			int exponent;
		};
		_tp<_tn T>
			FMT_API auto to_decimal(T x) noexcept -> decimal_fp<T>;
	} // namespace dragonbox
	_tp<_tn Float> _cx auto has_implicit_bit() -> bool {
		_rt std::numeric_limits<Float>::digits != 64;
	}
	_tp<_tn Float> _cx auto num_significand_bits() -> int {
		_rt is_float128<Float>() ? 112
								 : (std::numeric_limits<Float>::digits - (has_implicit_bit<Float>() ? 1 : 0));
	}
	_tp<_tn Float> _cx auto exponent_mask() -> _tn dragonbox::float_info<Float>::carrier_uint {
		using float_uint = _tn dragonbox::float_info<Float>::carrier_uint;
		_rt((float_uint(1) << dragonbox::float_info<Float>::exponent_bits) - 1)
			<< num_significand_bits<Float>();
	}
	_tp<_tn Float> _cx auto exponent_bias() -> int {
		_rt is_float128<Float>() ? 16383
								 : std::numeric_limits<Float>::max_exponent - 1;
	}
	_tp<_tn Char, _tn It> _cx auto write_exponent(int exp, It it) -> It {
		FMT_ASSERT(-10000 < exp && exp < 10000, "exponent out of range");
		if (exp < 0) {
			*it++ = _sc<Char>('-');
			exp = -exp;
		}
		else {
			*it++ = _sc<Char>('+');
		}
		if (exp >= 100) {
			const char* top = digits2(to_unsigned(exp / 100));
			if (exp >= 1000) *it++ = _sc<Char>(top[0]);
			*it++ = _sc<Char>(top[1]);
			exp %= 100;
		}
		const char* d = digits2(to_unsigned(exp));
		*it++ = _sc<Char>(d[0]);
		*it++ = _sc<Char>(d[1]);
		_rt it;
	}
	_tp<_tn F> struct basic_fp {
		F f;
		int e;
		static _cx const int num_significand_bits =
			_sc<int>(sizeof(F) * num_bits<unsigned char>());
		_cx basic_fp()
			: f(0), e(0) {}
		_cx basic_fp(uint64_t f_val, int e_val)
			: f(f_val), e(e_val) {}
		_tp<_tn Float> _cx basic_fp(Float n) { assign(n); }
		_tp<_tn Float, FMT_ENABLE_IF(!is_double_double<Float>::value)> _cx auto assign(Float n) -> bool {
			static_assert(std::numeric_limits<Float>::digits <= 113, "unsupported FP");
			using carrier_uint = _tn dragonbox::float_info<Float>::carrier_uint;
			const auto num_float_significand_bits =
				detail::num_significand_bits<Float>();
			const auto implicit_bit = carrier_uint(1) << num_float_significand_bits;
			const auto significand_mask = implicit_bit - 1;
			auto u = bit_cast<carrier_uint>(n);
			f = _sc<F>(u & significand_mask);
			auto biased_e = _sc<int>((u & exponent_mask<Float>()) >> num_float_significand_bits);
			auto is_predecessor_closer = f == 0 && biased_e > 1;
			if (biased_e == 0)
				biased_e = 1;
			else if (has_implicit_bit<Float>())
				f += _sc<F>(implicit_bit);
			e = biased_e - exponent_bias<Float>() - num_float_significand_bits;
			if (!has_implicit_bit<Float>()) ++e;
			_rt is_predecessor_closer;
		}
		_tp<_tn Float, FMT_ENABLE_IF(is_double_double<Float>::value)> _cx auto assign(Float n) -> bool {
			static_assert(std::numeric_limits<double>::is_iec559, "unsupported FP");
			_rt assign(_sc<double>(n));
		}
	};
	using fp = basic_fp<ull>;
	_tp<int SHIFT = 0, _tn F> _cx auto normalize(basic_fp<F> value) -> basic_fp<F> {
		const auto implicit_bit = F(1) << num_significand_bits<double>();
		const auto shifted_implicit_bit = implicit_bit << SHIFT;
		while ((value.f & shifted_implicit_bit) == 0) {
			value.f <<= 1;
			--value.e;
		}
		const auto offset = basic_fp<F>::num_significand_bits - num_significand_bits<double>() - SHIFT - 1;
		value.f <<= offset;
		value.e -= offset;
		_rt value;
	}
	_cx inline auto multiply(uint64_t lhs, uint64_t rhs) -> uint64_t {
#if FMT_USE_INT128
		auto product = _sc<__uint128_t>(lhs) * rhs;
		auto f = _sc<uint64_t>(product >> 64);
		_rt(_sc<uint64_t>(product) & (1ULL << 63)) != 0 ? f + 1 : f;
#else
		uint64_t mask = (1ULL << 32) - 1;
		uint64_t a = lhs >> 32, b = lhs & mask;
		uint64_t c = rhs >> 32, d = rhs & mask;
		uint64_t ac = a * c, bc = b * c, ad = a * d, bd = b * d;
		uint64_t mid = (bd >> 32) + (ad & mask) + (bc & mask) + (1U << 31);
		_rt ac + (ad >> 32) + (bc >> 32) + (mid >> 32);
#endif
	}
	_cx inline auto operator*(fp x, fp y) -> fp {
		_rt{ multiply(x.f, y.f), x.e + y.e + 64 };
	}
	_tp<_tn T, bool doublish = num_bits<T>() == num_bits<double>()> using convert_float_result =
		conditional_t < std::is_same<T, float>::value || doublish,
																		  double, T > ;
	_tp<_tn T> _cx auto convert_float(T value) -> convert_float_result<T> {
		_rt _sc<convert_float_result<T>>(value);
	}
	_tp<_tn Char, _tn OutputIt>
		FMT_NOINLINE _cx auto fill(OutputIt it, size_t n, const fill_t& fill)
			-> OutputIt {
		auto fill_size = fill.size();
		if (fill_size == 1) _rt detail::fill_n(it, n, fill._tp get<Char>());
		if (const Char* data = fill._tp data<Char>()) {
			for (size_t i = 0; i < n; ++i) it = copy<Char>(data, data + fill_size, it);
		}
		_rt it;
	}
	_tp<_tn Char, align::type align = align::left, _tn OutputIt,
		_tn F>
		_cx auto
		write_padded(OutputIt out, const format_specs& specs,
					 size_t size, size_t width, F&& f) -> OutputIt {
		static_assert(align == align::left || align == align::right, "");
		unsigned spec_width = to_unsigned(specs.width);
		size_t padding = spec_width > width ? spec_width - width : 0;
		auto* shifts = align == align::left ? "\x1f\x1f\x00\x01" : "\x00\x1f\x00\x01";
		size_t left_padding = padding >> shifts[specs.align];
		size_t right_padding = padding - left_padding;
		auto it = reserve(out, size + padding * specs.fill.size());
		if (left_padding != 0) it = fill<Char>(it, left_padding, specs.fill);
		it = f(it);
		if (right_padding != 0) it = fill<Char>(it, right_padding, specs.fill);
		_rt base_iterator(out, it);
	}
	_tp<_tn Char, align::type align = align::left, _tn OutputIt,
		_tn F>
		_cx auto
		write_padded(OutputIt out, const format_specs& specs,
					 size_t size, F&& f) -> OutputIt {
		_rt write_padded<Char, align>(out, specs, size, size, f);
	}
	_tp<_tn Char, align::type align = align::left, _tn OutputIt> _cx auto write_bytes(OutputIt out, string_view bytes,
																					  const format_specs& specs = {}) -> OutputIt {
		_rt write_padded<Char, align>(
			out, specs, bytes.size(), [bytes](reserve_iterator<OutputIt> it) {
				const char* data = bytes.data();
				_rt copy<Char>(data, data + bytes.size(), it);
			});
	}
	_tp<_tn Char, _tn OutputIt, _tn UIntPtr> auto write_ptr(OutputIt out, UIntPtr value, const format_specs* specs)
		-> OutputIt {
		int num_digits = count_digits<4>(value);
		auto size = to_unsigned(num_digits) + size_t(2);
		auto write = [=](reserve_iterator<OutputIt> it) {
			*it++ = _sc<Char>('0');
			*it++ = _sc<Char>('x');
			_rt format_uint<4, Char>(it, value, num_digits);
		};
		_rt specs ? write_padded<Char, align::right>(out, *specs, size, write)
				  : base_iterator(out, write(reserve(out, size)));
	}
	FMT_API auto is_printable(uint32_t cp) -> bool;
	inline auto needs_escape(uint32_t cp) -> bool {
		_rt cp < 0x20 || cp == 0x7f || cp == '"' || cp == '\\' || !is_printable(cp);
	}
	_tp<_tn Char> struct find_escape_result {
		const Char* begin;
		const Char* end;
		uint32_t cp;
	};
	_tp<_tn Char> auto find_escape(const Char* begin, const Char* end)
		-> find_escape_result<Char> {
		for (; begin != end; ++begin) {
			uint32_t cp = _sc<unsigned_char<Char>>(*begin);
			if (const_check(sizeof(Char) == 1) && cp >= 0x80) continue;
			if (needs_escape(cp)) _rt{ begin, begin + 1, cp };
		}
		_rt{ begin, nullptr, 0 };
	}
	inline auto find_escape(const char* begin, const char* end)
		-> find_escape_result<char> {
		if (!is_utf8()) _rt find_escape<char>(begin, end);
		auto result = find_escape_result<char>{ end, nullptr, 0 };
		for_each_codepoint(string_view(begin, to_unsigned(end - begin)),
						   [&](uint32_t cp, string_view sv) {
							   if (needs_escape(cp)) {
								   result = { sv.begin(), sv.end(), cp };
								   _rt false;
							   }
							   _rt true;
						   });
		_rt result;
	}
#define FMT_STRING_IMPL(s, base, explicit)                                          \
	[] {                                                                            \
		struct FMT_VISIBILITY("hidden") FMT_COMPILE_STRING : base {                 \
			using char_type FMT_MAYBE_UNUSED = fmt::remove_cvref_t<decltype(s[0])>; \
			FMT_MAYBE_UNUSED _cx explicit                                           \
			operator fmt::basic_string_view<char_type>() const {                    \
				_rt fmt::detail_exported::compile_string_to_view<char_type>(s);     \
			}                                                                       \
		};                                                                          \
		_rt FMT_COMPILE_STRING();                                                   \
	}()
#define FMT_STRING(s) FMT_STRING_IMPL(s, fmt::detail::compile_string, )
	_tp<size_t width, _tn Char, _tn OutputIt> auto write_codepoint(OutputIt out, char prefix, uint32_t cp) -> OutputIt {
		*out++ = _sc<Char>('\\');
		*out++ = _sc<Char>(prefix);
		Char buf[width];
		fill_n(buf, width, _sc<Char>('0'));
		format_uint<4>(buf, cp, width);
		_rt copy<Char>(buf, buf + width, out);
	}
	_tp<_tn OutputIt, _tn Char> auto write_escaped_cp(OutputIt out, const find_escape_result<Char>& escape)
		-> OutputIt {
		auto c = _sc<Char>(escape.cp);
		switch (escape.cp) {
		case '\n':
			*out++ = _sc<Char>('\\');
			c = _sc<Char>('n');
			break;
		case '\r':
			*out++ = _sc<Char>('\\');
			c = _sc<Char>('r');
			break;
		case '\t':
			*out++ = _sc<Char>('\\');
			c = _sc<Char>('t');
			break;
		case '"':
			FMT_FALLTHROUGH;
		case '\'':
			FMT_FALLTHROUGH;
		case '\\':
			*out++ = _sc<Char>('\\');
			break;
		default:
			if (escape.cp < 0x100) _rt write_codepoint<2, Char>(out, 'x', escape.cp);
			if (escape.cp < 0x10000)
				_rt write_codepoint<4, Char>(out, 'u', escape.cp);
			if (escape.cp < 0x110000)
				_rt write_codepoint<8, Char>(out, 'U', escape.cp);
			for (Char escape_char : basic_string_view<Char>(
					 escape.begin, to_unsigned(escape.end - escape.begin))) {
				out = write_codepoint<2, Char>(out, 'x',
											   _sc<uint32_t>(escape_char) & 0xFF);
			}
			_rt out;
		}
		*out++ = c;
		_rt out;
	}
	_tp<_tn Char, _tn OutputIt> auto write_escaped_string(OutputIt out, basic_string_view<Char> str)
		-> OutputIt {
		*out++ = _sc<Char>('"');
		auto begin = str.begin(), end = str.end();
		do {
			auto escape = find_escape(begin, end);
			out = copy<Char>(begin, escape.begin, out);
			begin = escape.end;
			if (!begin) break;
			out = write_escaped_cp<OutputIt, Char>(out, escape);
		} while (begin != end);
		*out++ = _sc<Char>('"');
		_rt out;
	}
	_tp<_tn Char, _tn OutputIt> auto write_escaped_char(OutputIt out, Char v) -> OutputIt {
		Char v_array[1] = { v };
		*out++ = _sc<Char>('\'');
		if ((needs_escape(_sc<uint32_t>(v)) && v != _sc<Char>('"')) || v == _sc<Char>('\'')) {
			out = write_escaped_cp(out,
								   find_escape_result<Char>{ v_array, v_array + 1,
															 _sc<uint32_t>(v) });
		}
		else {
			*out++ = v;
		}
		*out++ = _sc<Char>('\'');
		_rt out;
	}
	_tp<_tn Char, _tn OutputIt> _cx auto write_char(OutputIt out, Char value,
													const format_specs& specs) -> OutputIt {
		bool is_debug = specs.type == presentation_type::debug;
		_rt write_padded<Char>(out, specs, 1, [=](reserve_iterator<OutputIt> it) {
			if (is_debug) _rt write_escaped_char(it, value);
			*it++ = value;
			_rt it;
		});
	}
	_tp<_tn Char, _tn OutputIt> _cx auto write(OutputIt out, Char value, const format_specs& specs,
											   locale_ref loc = {}) -> OutputIt {
		using unsigned_type =
			conditional_t<std::is_same<Char, char>::value, unsigned char, unsigned>;
		_rt check_char_specs(specs)
			? write_char<Char>(out, value, specs)
			: write<Char>(out, _sc<unsigned_type>(value), specs, loc);
	}
	_tp<_tn Char> struct write_int_data {
		size_t size;
		size_t padding;
		_cx write_int_data(int num_digits, unsigned prefix,
						   const format_specs& specs)
			: size((prefix >> 24) + to_unsigned(num_digits)), padding(0) {
			if (specs.align == align::numeric) {
				auto width = to_unsigned(specs.width);
				if (width > size) {
					padding = width - size;
					size = width;
				}
			}
			else if (specs.precision > num_digits) {
				size = (prefix >> 24) + to_unsigned(specs.precision);
				padding = to_unsigned(specs.precision - num_digits);
			}
		}
	};
	_tp<_tn Char, _tn OutputIt, _tn W> _cx FMT_INLINE auto write_int(OutputIt out, int num_digits,
																	 unsigned prefix,
																	 const format_specs& specs,
																	 W write_digits) -> OutputIt {
		if ((specs.width | (specs.precision + 1)) == 0) {
			auto it = reserve(out, to_unsigned(num_digits) + (prefix >> 24));
			if (prefix != 0) {
				for (unsigned p = prefix & 0xffffff; p != 0; p >>= 8)
					*it++ = _sc<Char>(p & 0xff);
			}
			_rt base_iterator(out, write_digits(it));
		}
		auto data = write_int_data<Char>(num_digits, prefix, specs);
		_rt write_padded<Char, align::right>(
			out, specs, data.size, [=](reserve_iterator<OutputIt> it) {
				for (unsigned p = prefix & 0xffffff; p != 0; p >>= 8)
					*it++ = _sc<Char>(p & 0xff);
				it = detail::fill_n(it, data.padding, _sc<Char>('0'));
				_rt write_digits(it);
			});
	}
	_tp<_tn Char> class digit_grouping {
	private:
		std::string grouping_;
		std::basic_string<Char> thousands_sep_;
		struct next_state {
			std::string::const_iterator group;
			int pos;
		};
		auto initial_state() const -> next_state { _rt{ grouping_.begin(), 0 }; }
		auto next(next_state& state) const -> int {
			if (thousands_sep_.empty()) _rt max_value<int>();
			if (state.group == grouping_.end()) _rt state.pos += grouping_.back();
			if (*state.group <= 0 || *state.group == max_value<char>())
				_rt max_value<int>();
			state.pos += *state.group++;
			_rt state.pos;
		}

	public:
		explicit digit_grouping(locale_ref loc, bool localized = true) {
			if (!localized) _rt;
			auto sep = thousands_sep<Char>(loc);
			grouping_ = sep.grouping;
			if (sep.thousands_sep) thousands_sep_.assign(1, sep.thousands_sep);
		}
		digit_grouping(std::string grouping, std::basic_string<Char> sep)
			: grouping_(std::move(grouping)), thousands_sep_(std::move(sep)) {}
		auto has_separator() const -> bool { _rt !thousands_sep_.empty(); }
		auto count_separators(int num_digits) const -> int {
			int count = 0;
			auto state = initial_state();
			while (num_digits > next(state)) ++count;
			_rt count;
		}
		_tp<_tn Out, _tn C> auto apply(Out out, basic_string_view<C> digits) const -> Out {
			auto num_digits = _sc<int>(digits.size());
			auto separators = basic_memory_buffer<int>();
			separators.push_back(0);
			auto state = initial_state();
			while (int i = next(state)) {
				if (i >= num_digits) break;
				separators.push_back(i);
			}
			for (int i = 0, sep_index = _sc<int>(separators.size() - 1);
				 i < num_digits; ++i) {
				if (num_digits - i == separators[sep_index]) {
					out = copy<Char>(thousands_sep_.data(),
									 thousands_sep_.data() + thousands_sep_.size(), out);
					--sep_index;
				}
				*out++ = _sc<Char>(digits[to_unsigned(i)]);
			}
			_rt out;
		}
	};
	_cx inline void prefix_append(unsigned& prefix, unsigned value) {
		prefix |= prefix != 0 ? value << 8 : value;
		prefix += (1u + (value > 0xff ? 1 : 0)) << 24;
	}
	_tp<_tn OutputIt, _tn UInt, _tn Char> auto write_int(OutputIt out, UInt value, unsigned prefix,
														 const format_specs& specs, const digit_grouping<Char>& grouping)
		-> OutputIt {
		static_assert(std::is_same<uint64_or_128_t<UInt>, UInt>::value, "");
		int num_digits = 0;
		auto buffer = memory_buffer();
		switch (specs.type) {
		default:
			FMT_ASSERT(false, "");
			FMT_FALLTHROUGH;
		case presentation_type::none:
		case presentation_type::dec:
			num_digits = count_digits(value);
			format_decimal<char>(appender(buffer), value, num_digits);
			break;
		case presentation_type::hex:
			if (specs.alt)
				prefix_append(prefix, unsigned(specs.upper ? 'X' : 'x') << 8 | '0');
			num_digits = count_digits<4>(value);
			format_uint<4, char>(appender(buffer), value, num_digits, specs.upper);
			break;
		case presentation_type::oct:
			num_digits = count_digits<3>(value);
			if (specs.alt && specs.precision <= num_digits && value != 0)
				prefix_append(prefix, '0');
			format_uint<3, char>(appender(buffer), value, num_digits);
			break;
		case presentation_type::bin:
			if (specs.alt)
				prefix_append(prefix, unsigned(specs.upper ? 'B' : 'b') << 8 | '0');
			num_digits = count_digits<1>(value);
			format_uint<1, char>(appender(buffer), value, num_digits);
			break;
		case presentation_type::chr:
			_rt write_char<Char>(out, _sc<Char>(value), specs);
		}
		unsigned size = (prefix != 0 ? prefix >> 24 : 0) + to_unsigned(num_digits) + to_unsigned(grouping.count_separators(num_digits));
		_rt write_padded<Char, align::right>(
			out, specs, size, size, [&](reserve_iterator<OutputIt> it) {
				for (unsigned p = prefix & 0xffffff; p != 0; p >>= 8)
					*it++ = _sc<Char>(p & 0xff);
				_rt grouping.apply(it, string_view(buffer.data(), buffer.size()));
			});
	}
	FMT_API auto write_loc(appender out, loc_value value, const format_specs& specs,
						   locale_ref loc) -> bool;
	_tp<_tn OutputIt> inline auto write_loc(OutputIt, loc_value, const format_specs&, locale_ref)
		-> bool {
		_rt false;
	}
	_tp<_tn UInt> struct write_int_arg {
		UInt abs_value;
		unsigned prefix;
	};
	_tp<_tn T> _cx auto make_write_int_arg(T value, sign_t sign)
		-> write_int_arg<uint32_or_64_or_128_t<T>> {
		auto prefix = 0u;
		auto abs_value = _sc<uint32_or_64_or_128_t<T>>(value);
		if (is_negative(value)) {
			prefix = 0x01000000 | '-';
			abs_value = 0 - abs_value;
		}
		else {
			_cx const unsigned prefixes[4] = { 0, 0, 0x1000000u | '+',
											   0x1000000u | ' ' };
			prefix = prefixes[sign];
		}
		_rt{ abs_value, prefix };
	}
	_tp<_tn Char = char> struct loc_writer {
		basic_appender<Char> out;
		const format_specs& specs;
		std::basic_string<Char> sep;
		std::string grouping;
		std::basic_string<Char> decimal_point;
		_tp<_tn T, FMT_ENABLE_IF(is_integer<T>::value)> auto operator()(T value) -> bool {
			auto arg = make_write_int_arg(value, specs.sign);
			write_int(out, _sc<uint64_or_128_t<T>>(arg.abs_value), arg.prefix,
					  specs, digit_grouping<Char>(grouping, sep));
			_rt true;
		}
		_tp<_tn T, FMT_ENABLE_IF(!is_integer<T>::value)> auto operator()(T) -> bool {
			_rt false;
		}
	};
	_tp<_tn Char, _tn OutputIt, _tn T> _cx FMT_INLINE auto write_int(OutputIt out, write_int_arg<T> arg,
																	 const format_specs& specs, locale_ref)
		-> OutputIt {
		static_assert(std::is_same<T, uint32_or_64_or_128_t<T>>::value, "");
		auto abs_value = arg.abs_value;
		auto prefix = arg.prefix;
		switch (specs.type) {
		default:
			FMT_ASSERT(false, "");
			FMT_FALLTHROUGH;
		case presentation_type::none:
		case presentation_type::dec: {
			int num_digits = count_digits(abs_value);
			_rt write_int<Char>(
				out, num_digits, prefix, specs, [=](reserve_iterator<OutputIt> it) {
					_rt format_decimal<Char>(it, abs_value, num_digits).end;
				});
		}
		case presentation_type::hex: {
			if (specs.alt)
				prefix_append(prefix, unsigned(specs.upper ? 'X' : 'x') << 8 | '0');
			int num_digits = count_digits<4>(abs_value);
			_rt write_int<Char>(
				out, num_digits, prefix, specs, [=](reserve_iterator<OutputIt> it) {
					_rt format_uint<4, Char>(it, abs_value, num_digits, specs.upper);
				});
		}
		case presentation_type::oct: {
			int num_digits = count_digits<3>(abs_value);
			if (specs.alt && specs.precision <= num_digits && abs_value != 0)
				prefix_append(prefix, '0');
			_rt write_int<Char>(
				out, num_digits, prefix, specs, [=](reserve_iterator<OutputIt> it) {
					_rt format_uint<3, Char>(it, abs_value, num_digits);
				});
		}
		case presentation_type::bin: {
			if (specs.alt)
				prefix_append(prefix, unsigned(specs.upper ? 'B' : 'b') << 8 | '0');
			int num_digits = count_digits<1>(abs_value);
			_rt write_int<Char>(
				out, num_digits, prefix, specs, [=](reserve_iterator<OutputIt> it) {
					_rt format_uint<1, Char>(it, abs_value, num_digits);
				});
		}
		case presentation_type::chr:
			_rt write_char<Char>(out, _sc<Char>(abs_value), specs);
		}
		_rt out;
	}
	_tp<_tn Char, _tn OutputIt, _tn T> _cx FMT_NOINLINE auto write_int_noinline(OutputIt out,
																				write_int_arg<T> arg,
																				const format_specs& specs,
																				locale_ref loc) -> OutputIt {
		_rt write_int<Char>(out, arg, specs, loc);
	}
	_tp<_tn Char, _tn T,
		FMT_ENABLE_IF(is_integral<T>::value && !std::is_same<T, bool>::value && !std::is_same<T, Char>::value)>
		_cx FMT_INLINE auto
		write(basic_appender<Char> out, T value,
			  const format_specs& specs, locale_ref loc)
			-> basic_appender<Char> {
		if (specs.localized && write_loc(out, value, specs, loc)) _rt out;
		_rt write_int_noinline<Char>(out, make_write_int_arg(value, specs.sign),
									 specs, loc);
	}
	_tp<_tn Char, _tn OutputIt, _tn T,
		FMT_ENABLE_IF(is_integral<T>::value && !std::is_same<T, bool>::value && !std::is_same<T, Char>::value && !std::is_same<OutputIt, basic_appender<Char>>::value)>
		_cx FMT_INLINE auto
		write(OutputIt out, T value,
			  const format_specs& specs, locale_ref loc)
			-> OutputIt {
		if (specs.localized && write_loc(out, value, specs, loc)) _rt out;
		_rt write_int<Char>(out, make_write_int_arg(value, specs.sign), specs,
							loc);
	}
	class counting_iterator {
	private:
		size_t count_;

	public:
		using iterator_category = std::output_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using pointer = void;
		using reference = void;
		FMT_UNCHECKED_ITERATOR(counting_iterator);
		struct value_type {
			_tp<_tn T> _cx void operator=(const T&) {}
		};
		_cx counting_iterator()
			: count_(0) {}
		_cx auto count() const -> size_t { _rt count_; }
		_cx auto operator++() -> counting_iterator& {
			++count_;
			_rt* this;
		}
		_cx auto operator++(int) -> counting_iterator {
			auto it = *this;
			++*this;
			_rt it;
		}
		_cx friend auto operator+(counting_iterator it, difference_type n)
			-> counting_iterator {
			it.count_ += _sc<size_t>(n);
			_rt it;
		}
		_cx auto operator*() const -> value_type { _rt{}; }
	};
	_tp<_tn Char, _tn OutputIt> _cx auto write(OutputIt out, basic_string_view<Char> s,
											   const format_specs& specs) -> OutputIt {
		auto data = s.data();
		auto size = s.size();
		if (specs.precision >= 0 && to_unsigned(specs.precision) < size)
			size = code_point_index(s, to_unsigned(specs.precision));
		bool is_debug = specs.type == presentation_type::debug;
		size_t width = 0;
		if (is_debug) size = write_escaped_string(counting_iterator{}, s).count();
		if (specs.width != 0) {
			if (is_debug)
				width = size;
			else
				width = compute_width(basic_string_view<Char>(data, size));
		}
		_rt write_padded<Char>(out, specs, size, width,
							   [=](reserve_iterator<OutputIt> it) {
								   if (is_debug) _rt write_escaped_string(it, s);
								   _rt copy<Char>(data, data + size, it);
							   });
	}
	_tp<_tn Char, _tn OutputIt> _cx auto write(OutputIt out,
											   basic_string_view<type_identity_t<Char>> s,
											   const format_specs& specs, locale_ref) -> OutputIt {
		_rt write<Char>(out, s, specs);
	}
	_tp<_tn Char, _tn OutputIt> _cx auto write(OutputIt out, const Char* s, const format_specs& specs,
											   locale_ref) -> OutputIt {
		if (specs.type == presentation_type::pointer)
			_rt write_ptr<Char>(out, bit_cast<uintptr_t>(s), &specs);
		if (!s) report_error("string pointer is null");
		_rt write<Char>(out, basic_string_view<Char>(s), specs, {});
	}
	_tp<_tn Char, _tn OutputIt, _tn T,
		FMT_ENABLE_IF(is_integral<T>::value && !std::is_same<T, bool>::value && !std::is_same<T, Char>::value)>
		_cx auto
		write(OutputIt out, T value) -> OutputIt {
		auto abs_value = _sc<uint32_or_64_or_128_t<T>>(value);
		bool negative = is_negative(value);
		if (negative) abs_value = ~abs_value + 1;
		int num_digits = count_digits(abs_value);
		auto size = (negative ? 1 : 0) + _sc<size_t>(num_digits);
		auto it = reserve(out, size);
		if (auto ptr = to_pointer<Char>(it, size)) {
			if (negative) *ptr++ = _sc<Char>('-');
			format_decimal<Char>(ptr, abs_value, num_digits);
			_rt out;
		}
		if (negative) *it++ = _sc<Char>('-');
		it = format_decimal<Char>(it, abs_value, num_digits).end;
		_rt base_iterator(out, it);
	}
	_tp<_tn Char> _cx auto parse_align(const Char* begin, const Char* end,
									   format_specs& specs) -> const Char* {
		FMT_ASSERT(begin != end, "");
		auto align = align::none;
		auto p = begin + code_point_length(begin);
		if (end - p <= 0) p = begin;
		for (;;) {
			switch (to_ascii(*p)) {
			case '<':
				align = align::left;
				break;
			case '>':
				align = align::right;
				break;
			case '^':
				align = align::center;
				break;
			}
			if (align != align::none) {
				if (p != begin) {
					auto c = *begin;
					if (c == '}') _rt begin;
					if (c == '{') {
						report_error("invalid fill character '{'");
						_rt begin;
					}
					specs.fill = basic_string_view<Char>(begin, to_unsigned(p - begin));
					begin = p + 1;
				}
				else {
					++begin;
				}
				break;
			}
			else if (p == begin) {
				break;
			}
			p = begin;
		}
		specs.align = align;
		_rt begin;
	}
	enum class float_format : unsigned char {
		general,
		exp,
		fixed
	};
	struct float_specs {
		int precision;
		float_format format : 8;
		sign_t sign : 8;
		bool locale : 1;
		bool binary32 : 1;
		bool showpoint : 1;
	};
	_cx inline auto parse_float_type_spec(const format_specs& specs)
		-> float_specs {
		auto result = float_specs();
		result.showpoint = specs.alt;
		result.locale = specs.localized;
		switch (specs.type) {
		default:
			FMT_FALLTHROUGH;
		case presentation_type::none:
			result.format = float_format::general;
			break;
		case presentation_type::exp:
			result.format = float_format::exp;
			result.showpoint |= specs.precision != 0;
			break;
		case presentation_type::fixed:
			result.format = float_format::fixed;
			result.showpoint |= specs.precision != 0;
			break;
		case presentation_type::general:
			result.format = float_format::general;
			break;
		}
		_rt result;
	}
	_tp<_tn Char, _tn OutputIt> _cx auto write_nonfinite(OutputIt out, bool isnan,
														 format_specs specs, sign_t sign)
		-> OutputIt {
		auto str =
			isnan ? (specs.upper ? "NAN" : "nan") : (specs.upper ? "INF" : "inf");
		_cx size_t str_size = 3;
		auto size = str_size + (sign ? 1 : 0);
		const bool is_zero_fill =
			specs.fill.size() == 1 && specs.fill._tp get<Char>() == '0';
		if (is_zero_fill) specs.fill = ' ';
		_rt write_padded<Char>(out, specs, size,
							   [=](reserve_iterator<OutputIt> it) {
								   if (sign) *it++ = detail::sign<Char>(sign);
								   _rt copy<Char>(str, str + str_size, it);
							   });
	}
	struct big_decimal_fp {
		const char* significand;
		int significand_size;
		int exponent;
	};
	_cx auto get_significand_size(const big_decimal_fp& f) -> int {
		_rt f.significand_size;
	}
	_tp<_tn T> inline auto get_significand_size(const dragonbox::decimal_fp<T>& f) -> int {
		_rt count_digits(f.significand);
	}
	_tp<_tn Char, _tn OutputIt> _cx auto write_significand(OutputIt out, const char* significand,
														   int significand_size) -> OutputIt {
		_rt copy<Char>(significand, significand + significand_size, out);
	}
	_tp<_tn Char, _tn OutputIt, _tn UInt> inline auto write_significand(OutputIt out, UInt significand,
																		int significand_size) -> OutputIt {
		_rt format_decimal<Char>(out, significand, significand_size).end;
	}
	_tp<_tn Char, _tn OutputIt, _tn T, _tn Grouping> _cx auto write_significand(OutputIt out, T significand,
																				int significand_size, int exponent,
																				const Grouping& grouping) -> OutputIt {
		if (!grouping.has_separator()) {
			out = write_significand<Char>(out, significand, significand_size);
			_rt detail::fill_n(out, exponent, _sc<Char>('0'));
		}
		auto buffer = memory_buffer();
		write_significand<char>(appender(buffer), significand, significand_size);
		detail::fill_n(appender(buffer), exponent, '0');
		_rt grouping.apply(out, string_view(buffer.data(), buffer.size()));
	}
	_tp<_tn Char, _tn UInt,
		FMT_ENABLE_IF(std::is_integral<UInt>::value)> inline auto
	write_significand(Char* out, UInt significand, int significand_size,
					  int integral_size, Char decimal_point) -> Char* {
		if (!decimal_point)
			_rt format_decimal(out, significand, significand_size).end;
		out += significand_size + 1;
		Char* end = out;
		int floating_size = significand_size - integral_size;
		for (int i = floating_size / 2; i > 0; --i) {
			out -= 2;
			copy2(out, digits2(_sc<std::size_t>(significand % 100)));
			significand /= 100;
		}
		if (floating_size % 2 != 0) {
			*--out = _sc<Char>('0' + significand % 10);
			significand /= 10;
		}
		*--out = decimal_point;
		format_decimal(out - integral_size, significand, integral_size);
		_rt end;
	}
	_tp<_tn OutputIt, _tn UInt, _tn Char,
		FMT_ENABLE_IF(!std::is_pointer<remove_cvref_t<OutputIt>>::value)> inline auto
	write_significand(OutputIt out, UInt significand,
					  int significand_size, int integral_size,
					  Char decimal_point) -> OutputIt {
		Char buffer[digits10<UInt>() + 2];
		auto end = write_significand(buffer, significand, significand_size,
									 integral_size, decimal_point);
		_rt detail::copy_noinline<Char>(buffer, end, out);
	}
	_tp<_tn OutputIt, _tn Char> _cx auto write_significand(OutputIt out, const char* significand,
														   int significand_size, int integral_size,
														   Char decimal_point) -> OutputIt {
		out = detail::copy_noinline<Char>(significand, significand + integral_size,
										  out);
		if (!decimal_point) _rt out;
		*out++ = decimal_point;
		_rt detail::copy_noinline<Char>(significand + integral_size,
										significand + significand_size, out);
	}
	_tp<_tn OutputIt, _tn Char, _tn T, _tn Grouping> _cx auto write_significand(OutputIt out, T significand,
																				int significand_size, int integral_size,
																				Char decimal_point,
																				const Grouping& grouping) -> OutputIt {
		if (!grouping.has_separator()) {
			_rt write_significand(out, significand, significand_size, integral_size,
								  decimal_point);
		}
		auto buffer = basic_memory_buffer<Char>();
		write_significand(basic_appender<Char>(buffer), significand, significand_size,
						  integral_size, decimal_point);
		grouping.apply(
			out, basic_string_view<Char>(buffer.data(), to_unsigned(integral_size)));
		_rt detail::copy_noinline<Char>(buffer.data() + integral_size,
										buffer.end(), out);
	}
	_tp<_tn Char, _tn OutputIt, _tn DecimalFP,
		_tn Grouping = digit_grouping<Char>>
		_cx auto
		do_write_float(OutputIt out, const DecimalFP& f,
					   const format_specs& specs,
					   float_specs fspecs, locale_ref loc)
			-> OutputIt {
		auto significand = f.significand;
		int significand_size = get_significand_size(f);
		const Char zero = _sc<Char>('0');
		auto sign = fspecs.sign;
		size_t size = to_unsigned(significand_size) + (sign ? 1 : 0);
		using iterator = reserve_iterator<OutputIt>;
		Char decimal_point =
			fspecs.locale ? detail::decimal_point<Char>(loc) : _sc<Char>('.');
		int output_exp = f.exponent + significand_size - 1;
		auto use_exp_format = [=]() {
			if (fspecs.format == float_format::exp) _rt true;
			if (fspecs.format != float_format::general) _rt false;
			const int exp_lower = -4, exp_upper = 16;
			_rt output_exp < exp_lower || output_exp >= (fspecs.precision > 0 ? fspecs.precision : exp_upper);
		};
		if (use_exp_format()) {
			int num_zeros = 0;
			if (fspecs.showpoint) {
				num_zeros = fspecs.precision - significand_size;
				if (num_zeros < 0) num_zeros = 0;
				size += to_unsigned(num_zeros);
			}
			else if (significand_size == 1) {
				decimal_point = Char();
			}
			auto abs_output_exp = output_exp >= 0 ? output_exp : -output_exp;
			int exp_digits = 2;
			if (abs_output_exp >= 100) exp_digits = abs_output_exp >= 1000 ? 4 : 3;
			size += to_unsigned((decimal_point ? 1 : 0) + 2 + exp_digits);
			char exp_char = specs.upper ? 'E' : 'e';
			auto write = [=](iterator it) {
				if (sign) *it++ = detail::sign<Char>(sign);
				it = write_significand(it, significand, significand_size, 1,
									   decimal_point);
				if (num_zeros > 0) it = detail::fill_n(it, num_zeros, zero);
				*it++ = _sc<Char>(exp_char);
				_rt write_exponent<Char>(output_exp, it);
			};
			_rt specs.width > 0
				? write_padded<Char, align::right>(out, specs, size, write)
				: base_iterator(out, write(reserve(out, size)));
		}
		int exp = f.exponent + significand_size;
		if (f.exponent >= 0) {
			size += to_unsigned(f.exponent);
			int num_zeros = fspecs.precision - exp;
			abort_fuzzing_if(num_zeros > 5000);
			if (fspecs.showpoint) {
				++size;
				if (num_zeros <= 0 && fspecs.format != float_format::fixed) num_zeros = 0;
				if (num_zeros > 0) size += to_unsigned(num_zeros);
			}
			auto grouping = Grouping(loc, fspecs.locale);
			size += to_unsigned(grouping.count_separators(exp));
			_rt write_padded<Char, align::right>(out, specs, size, [&](iterator it) {
				if (sign) *it++ = detail::sign<Char>(sign);
				it = write_significand<Char>(it, significand, significand_size,
											 f.exponent, grouping);
				if (!fspecs.showpoint) _rt it;
				*it++ = decimal_point;
				_rt num_zeros > 0 ? detail::fill_n(it, num_zeros, zero) : it;
			});
		}
		else if (exp > 0) {
			int num_zeros = fspecs.showpoint ? fspecs.precision - significand_size : 0;
			size += 1 + to_unsigned(num_zeros > 0 ? num_zeros : 0);
			auto grouping = Grouping(loc, fspecs.locale);
			size += to_unsigned(grouping.count_separators(exp));
			_rt write_padded<Char, align::right>(out, specs, size, [&](iterator it) {
				if (sign) *it++ = detail::sign<Char>(sign);
				it = write_significand(it, significand, significand_size, exp,
									   decimal_point, grouping);
				_rt num_zeros > 0 ? detail::fill_n(it, num_zeros, zero) : it;
			});
		}
		int num_zeros = -exp;
		if (significand_size == 0 && fspecs.precision >= 0 && fspecs.precision < num_zeros) {
			num_zeros = fspecs.precision;
		}
		bool pointy = num_zeros != 0 || significand_size != 0 || fspecs.showpoint;
		size += 1 + (pointy ? 1 : 0) + to_unsigned(num_zeros);
		_rt write_padded<Char, align::right>(out, specs, size, [&](iterator it) {
			if (sign) *it++ = detail::sign<Char>(sign);
			*it++ = zero;
			if (!pointy) _rt it;
			*it++ = decimal_point;
			it = detail::fill_n(it, num_zeros, zero);
			_rt write_significand<Char>(it, significand, significand_size);
		});
	}
	_tp<_tn Char> class fallback_digit_grouping {
	public:
		_cx fallback_digit_grouping(locale_ref, bool) {}
		_cx auto has_separator() const -> bool { _rt false; }
		_cx auto count_separators(int) const -> int { _rt 0; }
		_tp<_tn Out, _tn C> _cx auto apply(Out out, basic_string_view<C>) const -> Out {
			_rt out;
		}
	};
	_tp<_tn Char, _tn OutputIt, _tn DecimalFP> _cx auto write_float(OutputIt out, const DecimalFP& f,
																	const format_specs& specs, float_specs fspecs,
																	locale_ref loc) -> OutputIt {
		if (is_constant_evaluated()) {
			_rt do_write_float<Char, OutputIt, DecimalFP,
							   fallback_digit_grouping<Char>>(out, f, specs, fspecs,
															  loc);
		}
		else {
			_rt do_write_float<Char>(out, f, specs, fspecs, loc);
		}
	}
	_tp<_tn T> _cx auto isnan(T value) -> bool {
		_rt !(value >= value);
	}
	_tp<_tn T, _tn Enable = void> struct has_isfinite : std::false_type {};
	_tp<_tn T> struct has_isfinite<T, enable_if_t<sizeof(std::isfinite(T())) != 0>>
		: std::true_type {};
	_tp<_tn T, FMT_ENABLE_IF(std::is_floating_point<T>::value&&
								 has_isfinite<T>::value)>
		_cx auto
		isfinite(T value) -> bool {
		_cx T inf = T(std::numeric_limits<double>::infinity());
		if (is_constant_evaluated())
			_rt !detail::isnan(value) && value < inf && value > -inf;
		_rt std::isfinite(value);
	}
	_tp<_tn T, FMT_ENABLE_IF(!has_isfinite<T>::value)> _cx auto isfinite(T value) -> bool {
		T inf = T(std::numeric_limits<double>::infinity());
		_rt !detail::isnan(value) && value < inf && value > -inf;
	}
	_tp<_tn T, FMT_ENABLE_IF(is_floating_point<T>::value)>
		FMT_INLINE _cx bool signbit(T value) {
		if (is_constant_evaluated()) {
#ifdef __cpp_if__cx
			if _cx (std::numeric_limits<double>::is_iec559) {
				auto bits = detail::bit_cast<uint64_t>(_sc<double>(value));
				_rt(bits >> (num_bits<uint64_t>() - 1)) != 0;
			}
#endif
		}
		_rt std::signbit(_sc<double>(value));
	}
	inline _cx void adjust_precision(int& precision, int exp10) {
		if (exp10 > 0 && precision > max_value<int>() - exp10)
			FMT_THROW(format_error("number is too big"));
		precision += exp10;
	}
	class bigint {
	private:
		using bigit = uint32_t;
		using double_bigit = uint64_t;
		enum { bigits_capacity = 32 };
		basic_memory_buffer<bigit, bigits_capacity> bigits_;
		int exp_;
		_cx auto operator[](int index) const -> bigit {
			_rt bigits_[to_unsigned(index)];
		}
		_cx auto operator[](int index) -> bigit& {
			_rt bigits_[to_unsigned(index)];
		}
		static _cx const int bigit_bits = num_bits<bigit>();
		friend struct formatter<bigint>;
		_cx void subtract_bigits(int index, bigit other, bigit& borrow) {
			auto result = _sc<double_bigit>((*this)[index]) - other - borrow;
			(*this)[index] = _sc<bigit>(result);
			borrow = _sc<bigit>(result >> (bigit_bits * 2 - 1));
		}
		_cx void remove_leading_zeros() {
			int num_bigits = _sc<int>(bigits_.size()) - 1;
			while (num_bigits > 0 && (*this)[num_bigits] == 0) --num_bigits;
			bigits_.resize(to_unsigned(num_bigits + 1));
		}
		_cx void subtract_aligned(const bigint& other) {
			FMT_ASSERT(other.exp_ >= exp_, "unaligned bigints");
			FMT_ASSERT(compare(*this, other) >= 0, "");
			bigit borrow = 0;
			int i = other.exp_ - exp_;
			for (size_t j = 0, n = other.bigits_.size(); j != n; ++i, ++j)
				subtract_bigits(i, other.bigits_[j], borrow);
			while (borrow > 0) subtract_bigits(i, 0, borrow);
			remove_leading_zeros();
		}
		_cx void multiply(uint32_t value) {
			const double_bigit wide_value = value;
			bigit carry = 0;
			for (size_t i = 0, n = bigits_.size(); i < n; ++i) {
				double_bigit result = bigits_[i] * wide_value + carry;
				bigits_[i] = _sc<bigit>(result);
				carry = _sc<bigit>(result >> bigit_bits);
			}
			if (carry != 0) bigits_.push_back(carry);
		}
		_tp<_tn UInt, FMT_ENABLE_IF(std::is_same<UInt, uint64_t>::value || std::is_same<UInt, uint128_t>::value)> _cx void multiply(UInt value) {
			using half_uint =
				conditional_t<std::is_same<UInt, uint128_t>::value, uint64_t, uint32_t>;
			const int shift = num_bits<half_uint>() - bigit_bits;
			const UInt lower = _sc<half_uint>(value);
			const UInt upper = value >> num_bits<half_uint>();
			UInt carry = 0;
			for (size_t i = 0, n = bigits_.size(); i < n; ++i) {
				UInt result = lower * bigits_[i] + _sc<bigit>(carry);
				carry = (upper * bigits_[i] << shift) + (result >> bigit_bits) + (carry >> bigit_bits);
				bigits_[i] = _sc<bigit>(result);
			}
			while (carry != 0) {
				bigits_.push_back(_sc<bigit>(carry));
				carry >>= bigit_bits;
			}
		}
		_tp<_tn UInt, FMT_ENABLE_IF(std::is_same<UInt, uint64_t>::value || std::is_same<UInt, uint128_t>::value)> _cx void assign(UInt n) {
			size_t num_bigits = 0;
			do {
				bigits_[num_bigits++] = _sc<bigit>(n);
				n >>= bigit_bits;
			} while (n != 0);
			bigits_.resize(num_bigits);
			exp_ = 0;
		}

	public:
		_cx bigint()
			: exp_(0) {}
		explicit bigint(uint64_t n) { assign(n); }
		bigint(const bigint&) = delete;
		void operator=(const bigint&) = delete;
		_cx void assign(const bigint& other) {
			auto size = other.bigits_.size();
			bigits_.resize(size);
			auto data = other.bigits_.data();
			copy<bigit>(data, data + size, bigits_.data());
			exp_ = other.exp_;
		}
		_tp<_tn Int> _cx void operator=(Int n) {
			FMT_ASSERT(n > 0, "");
			assign(uint64_or_128_t<Int>(n));
		}
		_cx auto num_bigits() const -> int {
			_rt _sc<int>(bigits_.size()) + exp_;
		}
		FMT_NOINLINE _cx auto operator<<=(int shift) -> bigint& {
			FMT_ASSERT(shift >= 0, "");
			exp_ += shift / bigit_bits;
			shift %= bigit_bits;
			if (shift == 0) _rt* this;
			bigit carry = 0;
			for (size_t i = 0, n = bigits_.size(); i < n; ++i) {
				bigit c = bigits_[i] >> (bigit_bits - shift);
				bigits_[i] = (bigits_[i] << shift) + carry;
				carry = c;
			}
			if (carry != 0) bigits_.push_back(carry);
			_rt* this;
		}
		_tp<_tn Int> _cx auto operator*=(Int value) -> bigint& {
			FMT_ASSERT(value > 0, "");
			multiply(uint32_or_64_or_128_t<Int>(value));
			_rt* this;
		}
		friend _cx auto compare(const bigint& lhs, const bigint& rhs)
			-> int {
			int num_lhs_bigits = lhs.num_bigits(), num_rhs_bigits = rhs.num_bigits();
			if (num_lhs_bigits != num_rhs_bigits)
				_rt num_lhs_bigits > num_rhs_bigits ? 1 : -1;
			int i = _sc<int>(lhs.bigits_.size()) - 1;
			int j = _sc<int>(rhs.bigits_.size()) - 1;
			int end = i - j;
			if (end < 0) end = 0;
			for (; i >= end; --i, --j) {
				bigit lhs_bigit = lhs[i], rhs_bigit = rhs[j];
				if (lhs_bigit != rhs_bigit) _rt lhs_bigit > rhs_bigit ? 1 : -1;
			}
			if (i != j) _rt i > j ? 1 : -1;
			_rt 0;
		}
		friend _cx auto add_compare(const bigint& lhs1,
									const bigint& lhs2, const bigint& rhs)
			-> int {
			auto minimum = [](int a, int b) { _rt a < b ? a : b; };
			auto maximum = [](int a, int b) { _rt a > b ? a : b; };
			int max_lhs_bigits = maximum(lhs1.num_bigits(), lhs2.num_bigits());
			int num_rhs_bigits = rhs.num_bigits();
			if (max_lhs_bigits + 1 < num_rhs_bigits) _rt - 1;
			if (max_lhs_bigits > num_rhs_bigits) _rt 1;
			auto get_bigit = [](const bigint& n, int i) -> bigit {
				_rt i >= n.exp_&& i < n.num_bigits() ? n[i - n.exp_] : 0;
			};
			double_bigit borrow = 0;
			int min_exp = minimum(minimum(lhs1.exp_, lhs2.exp_), rhs.exp_);
			for (int i = num_rhs_bigits - 1; i >= min_exp; --i) {
				double_bigit sum =
					_sc<double_bigit>(get_bigit(lhs1, i)) + get_bigit(lhs2, i);
				bigit rhs_bigit = get_bigit(rhs, i);
				if (sum > rhs_bigit + borrow) _rt 1;
				borrow = rhs_bigit + borrow - sum;
				if (borrow > 1) _rt - 1;
				borrow <<= bigit_bits;
			}
			_rt borrow != 0 ? -1 : 0;
		}
		_cx void assign_pow10(int exp) {
			FMT_ASSERT(exp >= 0, "");
			if (exp == 0) _rt* this = 1;
			int bitmask = 1;
			while (exp >= bitmask) bitmask <<= 1;
			bitmask >>= 1;
			*this = 5;
			bitmask >>= 1;
			while (bitmask != 0) {
				square();
				if ((exp & bitmask) != 0) *this *= 5;
				bitmask >>= 1;
			}
			*this <<= exp;
		}
		_cx void square() {
			int num_bigits = _sc<int>(bigits_.size());
			int num_result_bigits = 2 * num_bigits;
			basic_memory_buffer<bigit, bigits_capacity> n(std::move(bigits_));
			bigits_.resize(to_unsigned(num_result_bigits));
			auto sum = uint128_t();
			for (int bigit_index = 0; bigit_index < num_bigits; ++bigit_index) {
				for (int i = 0, j = bigit_index; j >= 0; ++i, --j) {
					sum += _sc<double_bigit>(n[i]) * n[j];
				}
				(*this)[bigit_index] = _sc<bigit>(sum);
				sum >>= num_bits<bigit>();
			}
			for (int bigit_index = num_bigits; bigit_index < num_result_bigits;
				 ++bigit_index) {
				for (int j = num_bigits - 1, i = bigit_index - j; i < num_bigits;)
					sum += _sc<double_bigit>(n[i++]) * n[j--];
				(*this)[bigit_index] = _sc<bigit>(sum);
				sum >>= num_bits<bigit>();
			}
			remove_leading_zeros();
			exp_ *= 2;
		}
		_cx void align(const bigint& other) {
			int exp_difference = exp_ - other.exp_;
			if (exp_difference <= 0) _rt;
			int num_bigits = _sc<int>(bigits_.size());
			bigits_.resize(to_unsigned(num_bigits + exp_difference));
			for (int i = num_bigits - 1, j = i + exp_difference; i >= 0; --i, --j)
				bigits_[j] = bigits_[i];
			memset(bigits_.data(), 0, to_unsigned(exp_difference) * sizeof(bigit));
			exp_ -= exp_difference;
		}
		_cx auto divmod_assign(const bigint& divisor) -> int {
			FMT_ASSERT(this != &divisor, "");
			if (compare(*this, divisor) < 0) _rt 0;
			FMT_ASSERT(divisor.bigits_[divisor.bigits_.size() - 1u] != 0, "");
			align(divisor);
			int quotient = 0;
			do {
				subtract_aligned(divisor);
				++quotient;
			} while (compare(*this, divisor) >= 0);
			_rt quotient;
		}
	};
	enum dragon {
		predecessor_closer = 1,
		fixup = 2,
		fixed = 4,
	};
	_cx inline void format_dragon(basic_fp<uint128_t> value,
								  unsigned flags, int num_digits,
								  buffer<char>& buf, int& exp10) {
		bigint numerator;
		bigint denominator;
		bigint lower;
		bigint upper_store;
		bigint* upper = nullptr;
		bool is_predecessor_closer = (flags & dragon::predecessor_closer) != 0;
		int shift = is_predecessor_closer ? 2 : 1;
		if (value.e >= 0) {
			numerator = value.f;
			numerator <<= value.e + shift;
			lower = 1;
			lower <<= value.e;
			if (is_predecessor_closer) {
				upper_store = 1;
				upper_store <<= value.e + 1;
				upper = &upper_store;
			}
			denominator.assign_pow10(exp10);
			denominator <<= shift;
		}
		else if (exp10 < 0) {
			numerator.assign_pow10(-exp10);
			lower.assign(numerator);
			if (is_predecessor_closer) {
				upper_store.assign(numerator);
				upper_store <<= 1;
				upper = &upper_store;
			}
			numerator *= value.f;
			numerator <<= shift;
			denominator = 1;
			denominator <<= shift - value.e;
		}
		else {
			numerator = value.f;
			numerator <<= shift;
			denominator.assign_pow10(exp10);
			denominator <<= shift - value.e;
			lower = 1;
			if (is_predecessor_closer) {
				upper_store = 1ULL << 1;
				upper = &upper_store;
			}
		}
		int even = _sc<int>((value.f & 1) == 0);
		if (!upper) upper = &lower;
		bool shortest = num_digits < 0;
		if ((flags & dragon::fixup) != 0) {
			if (add_compare(numerator, *upper, denominator) + even <= 0) {
				--exp10;
				numerator *= 10;
				if (num_digits < 0) {
					lower *= 10;
					if (upper != &lower) *upper *= 10;
				}
			}
			if ((flags & dragon::fixed) != 0) adjust_precision(num_digits, exp10 + 1);
		}
		if (shortest) {
			num_digits = 0;
			char* data = buf.data();
			for (;;) {
				int digit = numerator.divmod_assign(denominator);
				bool low = compare(numerator, lower) - even < 0;
				bool high = add_compare(numerator, *upper, denominator) + even > 0;
				data[num_digits++] = _sc<char>('0' + digit);
				if (low || high) {
					if (!low) {
						++data[num_digits - 1];
					}
					else if (high) {
						int result = add_compare(numerator, numerator, denominator);
						if (result > 0 || (result == 0 && (digit % 2) != 0))
							++data[num_digits - 1];
					}
					buf.try_resize(to_unsigned(num_digits));
					exp10 -= num_digits - 1;
					_rt;
				}
				numerator *= 10;
				lower *= 10;
				if (upper != &lower) *upper *= 10;
			}
		}
		exp10 -= num_digits - 1;
		if (num_digits <= 0) {
			auto digit = '0';
			if (num_digits == 0) {
				denominator *= 10;
				digit = add_compare(numerator, numerator, denominator) > 0 ? '1' : '0';
			}
			buf.push_back(digit);
			_rt;
		}
		buf.try_resize(to_unsigned(num_digits));
		for (int i = 0; i < num_digits - 1; ++i) {
			int digit = numerator.divmod_assign(denominator);
			buf[i] = _sc<char>('0' + digit);
			numerator *= 10;
		}
		int digit = numerator.divmod_assign(denominator);
		auto result = add_compare(numerator, numerator, denominator);
		if (result > 0 || (result == 0 && (digit % 2) != 0)) {
			if (digit == 9) {
				const auto overflow = '0' + 10;
				buf[num_digits - 1] = overflow;
				for (int i = num_digits - 1; i > 0 && buf[i] == overflow; --i) {
					buf[i] = '0';
					++buf[i - 1];
				}
				if (buf[0] == overflow) {
					buf[0] = '1';
					if ((flags & dragon::fixed) != 0)
						buf.push_back('0');
					else
						++exp10;
				}
				_rt;
			}
			++digit;
		}
		buf[num_digits - 1] = _sc<char>('0' + digit);
	}
	_tp<_tn Float, FMT_ENABLE_IF(!is_double_double<Float>::value)> _cx void format_hexfloat(Float value, format_specs specs,
																							buffer<char>& buf) {
		static_assert(!std::is_same<Float, float>::value, "");
		using info = dragonbox::float_info<Float>;
		using carrier_uint = _tn info::carrier_uint;
		_cx auto num_float_significand_bits =
			detail::num_significand_bits<Float>();
		basic_fp<carrier_uint> f(value);
		f.e += num_float_significand_bits;
		if (!has_implicit_bit<Float>()) --f.e;
		_cx auto num_fraction_bits =
			num_float_significand_bits + (has_implicit_bit<Float>() ? 1 : 0);
		_cx auto num_xdigits = (num_fraction_bits + 3) / 4;
		_cx auto leading_shift = ((num_xdigits - 1) * 4);
		const auto leading_mask = carrier_uint(0xF) << leading_shift;
		const auto leading_xdigit =
			_sc<uint32_t>((f.f & leading_mask) >> leading_shift);
		if (leading_xdigit > 1) f.e -= (32 - countl_zero(leading_xdigit) - 1);
		int print_xdigits = num_xdigits - 1;
		if (specs.precision >= 0 && print_xdigits > specs.precision) {
			const int shift = ((print_xdigits - specs.precision - 1) * 4);
			const auto mask = carrier_uint(0xF) << shift;
			const auto v = _sc<uint32_t>((f.f & mask) >> shift);
			if (v >= 8) {
				const auto inc = carrier_uint(1) << (shift + 4);
				f.f += inc;
				f.f &= ~(inc - 1);
			}
			if (!has_implicit_bit<Float>()) {
				const auto implicit_bit = carrier_uint(1) << num_float_significand_bits;
				if ((f.f & implicit_bit) == implicit_bit) {
					f.f >>= 4;
					f.e += 4;
				}
			}
			print_xdigits = specs.precision;
		}
		char xdigits[num_bits<carrier_uint>() / 4];
		detail::fill_n(xdigits, sizeof(xdigits), '0');
		format_uint<4>(xdigits, f.f, num_xdigits, specs.upper);
		while (print_xdigits > 0 && xdigits[print_xdigits] == '0') --print_xdigits;
		buf.push_back('0');
		buf.push_back(specs.upper ? 'X' : 'x');
		buf.push_back(xdigits[0]);
		if (specs.alt || print_xdigits > 0 || print_xdigits < specs.precision)
			buf.push_back('.');
		buf.append(xdigits + 1, xdigits + 1 + print_xdigits);
		for (; print_xdigits < specs.precision; ++print_xdigits) buf.push_back('0');
		buf.push_back(specs.upper ? 'P' : 'p');
		uint32_t abs_e;
		if (f.e < 0) {
			buf.push_back('-');
			abs_e = _sc<uint32_t>(-f.e);
		}
		else {
			buf.push_back('+');
			abs_e = _sc<uint32_t>(f.e);
		}
		format_decimal<char>(appender(buf), abs_e, detail::count_digits(abs_e));
	}
	_tp<_tn Float, FMT_ENABLE_IF(is_double_double<Float>::value)> _cx void format_hexfloat(Float value, format_specs specs,
																						   buffer<char>& buf) {
		format_hexfloat(_sc<double>(value), specs, buf);
	}
	_cx auto fractional_part_rounding_thresholds(int index) -> uint32_t {
		_rt U"\x9999999a\x828f5c29\x80418938\x80068db9\x8000a7c6\x800010c7"
			U"\x800001ae\x8000002b"[index];
	}
	_tp<_tn Float> _cx auto format_float(Float value, int precision, float_specs specs,
										 buffer<char>& buf) -> int {
		static_assert(!std::is_same<Float, float>::value, "");
		FMT_ASSERT(value >= 0, "value is negative");
		auto converted_value = convert_float(value);
		const bool fixed = specs.format == float_format::fixed;
		if (value <= 0) {
			if (precision <= 0 || !fixed) {
				buf.push_back('0');
				_rt 0;
			}
			buf.try_resize(to_unsigned(precision));
			fill_n(buf.data(), precision, '0');
			_rt - precision;
		}
		int exp = 0;
		bool use_dragon = true;
		unsigned dragon_flags = 0;
		if (!is_fast_float<Float>() || is_constant_evaluated()) {
			const auto inv_log2_10 = 0.3010299956639812;
			using info = dragonbox::float_info<decltype(converted_value)>;
			const auto f = basic_fp<_tn info::carrier_uint>(converted_value);
			auto e = (f.e + count_digits<1>(f.f) - 1) * inv_log2_10 - 1e-10;
			exp = _sc<int>(e);
			if (e > exp) ++exp;
			dragon_flags = dragon::fixup;
		}
		else if (precision < 0) {
			if (specs.binary32) {
				auto dec = dragonbox::to_decimal(_sc<float>(value));
				write<char>(appender(buf), dec.significand);
				_rt dec.exponent;
			}
			auto dec = dragonbox::to_decimal(_sc<double>(value));
			write<char>(appender(buf), dec.significand);
			_rt dec.exponent;
		}
		else {
			using info = dragonbox::float_info<double>;
			auto br = bit_cast<uint64_t>(_sc<double>(value));
			const uint64_t significand_mask =
				(_sc<uint64_t>(1) << num_significand_bits<double>()) - 1;
			uint64_t significand = (br & significand_mask);
			int exponent = _sc<int>((br & exponent_mask<double>()) >> num_significand_bits<double>());
			if (exponent != 0) {
				exponent -= exponent_bias<double>() + num_significand_bits<double>();
				significand |=
					(_sc<uint64_t>(1) << num_significand_bits<double>());
				significand <<= 1;
			}
			else {
				FMT_ASSERT(significand != 0, "zeros should not appear here");
				int shift = countl_zero(significand);
				FMT_ASSERT(shift >= num_bits<uint64_t>() - num_significand_bits<double>(),
						   "");
				shift -= (num_bits<uint64_t>() - num_significand_bits<double>() - 2);
				exponent = (std::numeric_limits<double>::min_exponent - num_significand_bits<double>()) - shift;
				significand <<= shift;
			}
			const int k = info::kappa - dragonbox::floor_log10_pow2(exponent);
			exp = -k;
			const int beta = exponent + dragonbox::floor_log2_pow10(k);
			uint64_t first_segment;
			bool has_more_segments;
			int digits_in_the_first_segment;
			{
				const auto r = dragonbox::umul192_upper128(
					significand << beta, dragonbox::get_cached_power(k));
				first_segment = r.high();
				has_more_segments = r.low() != 0;
				if (first_segment >= 1000000000000000000ULL) {
					digits_in_the_first_segment = 19;
				}
				else {
					digits_in_the_first_segment = 18;
					first_segment *= 10;
				}
			}
			if (fixed) adjust_precision(precision, exp + digits_in_the_first_segment);
			if (digits_in_the_first_segment > precision) {
				use_dragon = false;
				if (precision <= 0) {
					exp += digits_in_the_first_segment;
					if (precision < 0) {
						buf.try_resize(0);
					}
					else {
						buf.try_resize(1);
						if ((first_segment | _sc<uint64_t>(has_more_segments)) > 5000000000000000000ULL) {
							buf[0] = '1';
						}
						else {
							buf[0] = '0';
						}
					}
				}
				else {
					exp += digits_in_the_first_segment - precision;
					const uint32_t first_subsegment = _sc<uint32_t>(
						dragonbox::umul128_upper64(first_segment, 7922816251426433760ULL) >> 32);
					const uint64_t second_third_subsegments =
						first_segment - first_subsegment * 10000000000ULL;
					uint64_t prod;
					uint32_t digits;
					bool should_round_up;
					int number_of_digits_to_print = precision > 9 ? 9 : precision;
					auto print_subsegment = [&](uint32_t subsegment, char* buffer) {
						int number_of_digits_printed = 0;
						if ((number_of_digits_to_print & 1) != 0) {
							prod = ((subsegment * _sc<uint64_t>(720575941)) >> 24) + 1;
							digits = _sc<uint32_t>(prod >> 32);
							*buffer = _sc<char>('0' + digits);
							number_of_digits_printed++;
						}
						else {
							prod = ((subsegment * _sc<uint64_t>(450359963)) >> 20) + 1;
							digits = _sc<uint32_t>(prod >> 32);
							copy2(buffer, digits2(digits));
							number_of_digits_printed += 2;
						}
						while (number_of_digits_printed < number_of_digits_to_print) {
							prod = _sc<uint32_t>(prod) * _sc<uint64_t>(100);
							digits = _sc<uint32_t>(prod >> 32);
							copy2(buffer + number_of_digits_printed, digits2(digits));
							number_of_digits_printed += 2;
						}
					};
					print_subsegment(first_subsegment, buf.data());
					if (precision <= 9) {
						if (precision < 9) {
							uint32_t fractional_part = _sc<uint32_t>(prod);
							should_round_up =
								fractional_part >= fractional_part_rounding_thresholds(
									8 - number_of_digits_to_print)
								|| ((fractional_part >> 31) & ((digits & 1) | (second_third_subsegments != 0) | has_more_segments)) != 0;
						}
						else {
							should_round_up = second_third_subsegments > 5000000000ULL || (second_third_subsegments == 5000000000ULL && ((digits & 1) != 0 || has_more_segments));
						}
					}
					else {
						const uint32_t second_subsegment =
							_sc<uint32_t>(dragonbox::umul128_upper64(
								second_third_subsegments, 1844674407370955162ULL));
						const uint32_t third_subsegment =
							_sc<uint32_t>(second_third_subsegments) - second_subsegment * 10;
						number_of_digits_to_print = precision - 9;
						print_subsegment(second_subsegment, buf.data() + 9);
						if (precision < 18) {
							uint32_t fractional_part = _sc<uint32_t>(prod);
							should_round_up =
								fractional_part >= fractional_part_rounding_thresholds(
									8 - number_of_digits_to_print)
								|| ((fractional_part >> 31) & ((digits & 1) | (third_subsegment != 0) | has_more_segments)) != 0;
						}
						else {
							should_round_up = third_subsegment > 5 || (third_subsegment == 5 && ((digits & 1) != 0 || has_more_segments));
						}
					}
					if (should_round_up) {
						++buf[precision - 1];
						for (int i = precision - 1; i > 0 && buf[i] > '9'; --i) {
							buf[i] = '0';
							++buf[i - 1];
						}
						if (buf[0] > '9') {
							buf[0] = '1';
							if (fixed)
								buf[precision++] = '0';
							else
								++exp;
						}
					}
					buf.try_resize(to_unsigned(precision));
				}
			}
			else {
				exp += digits_in_the_first_segment - 1;
			}
		}
		if (use_dragon) {
			auto f = basic_fp<uint128_t>();
			bool is_predecessor_closer = specs.binary32
											 ? f.assign(_sc<float>(value))
											 : f.assign(converted_value);
			if (is_predecessor_closer) dragon_flags |= dragon::predecessor_closer;
			if (fixed) dragon_flags |= dragon::fixed;
			const int max_double_digits = 767;
			if (precision > max_double_digits) precision = max_double_digits;
			format_dragon(f, dragon_flags, precision, buf, exp);
		}
		if (!fixed && !specs.showpoint) {
			auto num_digits = buf.size();
			while (num_digits > 0 && buf[num_digits - 1] == '0') {
				--num_digits;
				++exp;
			}
			buf.try_resize(num_digits);
		}
		_rt exp;
	}
	_tp<_tn Char, _tn OutputIt, _tn T> _cx auto write_float(OutputIt out, T value, format_specs specs,
															locale_ref loc) -> OutputIt {
		sign_t sign = specs.sign;
		if (detail::signbit(value)) {
			sign = sign::minus;
			value = -value;
		}
		else if (sign == sign::minus) {
			sign = sign::none;
		}
		if (!detail::isfinite(value))
			_rt write_nonfinite<Char>(out, detail::isnan(value), specs, sign);
		if (specs.align == align::numeric && sign) {
			auto it = reserve(out, 1);
			*it++ = detail::sign<Char>(sign);
			out = base_iterator(out, it);
			sign = sign::none;
			if (specs.width != 0) --specs.width;
		}
		memory_buffer buffer;
		if (specs.type == presentation_type::hexfloat) {
			if (sign) buffer.push_back(detail::sign<char>(sign));
			format_hexfloat(convert_float(value), specs, buffer);
			_rt write_bytes<Char, align::right>(out, { buffer.data(), buffer.size() },
												specs);
		}
		int precision = specs.precision >= 0 || specs.type == presentation_type::none
							? specs.precision
							: 6;
		if (specs.type == presentation_type::exp) {
			if (precision == max_value<int>())
				report_error("number is too big");
			else
				++precision;
		}
		else if (specs.type != presentation_type::fixed && precision == 0) {
			precision = 1;
		}
		float_specs fspecs = parse_float_type_spec(specs);
		fspecs.sign = sign;
		if (const_check(std::is_same<T, float>())) fspecs.binary32 = true;
		int exp = format_float(convert_float(value), precision, fspecs, buffer);
		fspecs.precision = precision;
		auto f = big_decimal_fp{ buffer.data(), _sc<int>(buffer.size()), exp };
		_rt write_float<Char>(out, f, specs, fspecs, loc);
	}
	_tp<_tn Char, _tn OutputIt, _tn T,
		FMT_ENABLE_IF(is_floating_point<T>::value)>
		_cx auto
		write(OutputIt out, T value, format_specs specs,
			  locale_ref loc = {}) -> OutputIt {
		if (const_check(!is_supported_floating_point(value))) _rt out;
		_rt specs.localized&& write_loc(out, value, specs, loc)
			? out
			: write_float<Char>(out, value, specs, loc);
	}
	_tp<_tn Char, _tn OutputIt, _tn T,
		FMT_ENABLE_IF(is_fast_float<T>::value)>
		_cx auto
		write(OutputIt out, T value) -> OutputIt {
		if (is_constant_evaluated()) _rt write<Char>(out, value, format_specs());
		if (const_check(!is_supported_floating_point(value))) _rt out;
		auto sign = sign_t::none;
		if (detail::signbit(value)) {
			sign = sign::minus;
			value = -value;
		}
		_cx auto specs = format_specs();
		using floaty = conditional_t<std::is_same<T, long double>::value, double, T>;
		using floaty_uint = _tn dragonbox::float_info<floaty>::carrier_uint;
		floaty_uint mask = exponent_mask<floaty>();
		if ((bit_cast<floaty_uint>(value) & mask) == mask)
			_rt write_nonfinite<Char>(out, std::isnan(value), specs, sign);
		auto fspecs = float_specs();
		fspecs.sign = sign;
		auto dec = dragonbox::to_decimal(_sc<floaty>(value));
		_rt write_float<Char>(out, dec, specs, fspecs, {});
	}
	_tp<_tn Char, _tn OutputIt, _tn T,
		FMT_ENABLE_IF(is_floating_point<T>::value && !is_fast_float<T>::value)> inline auto
	write(OutputIt out, T value) -> OutputIt {
		_rt write<Char>(out, value, format_specs());
	}
	_tp<_tn Char, _tn OutputIt> auto write(OutputIt out, monostate, format_specs = {}, locale_ref = {})
		-> OutputIt {
		FMT_ASSERT(false, "");
		_rt out;
	}
	_tp<_tn Char, _tn OutputIt> _cx auto write(OutputIt out, basic_string_view<Char> value)
		-> OutputIt {
		auto it = reserve(out, value.size());
		it = copy_noinline<Char>(value.begin(), value.end(), it);
		_rt base_iterator(out, it);
	}
	_tp<_tn Char, _tn OutputIt, _tn T,
		FMT_ENABLE_IF(has_to_string_view<T>::value)>
		_cx auto
		write(OutputIt out, const T& value) -> OutputIt {
		_rt write<Char>(out, to_string_view(value));
	}
	_tp<
		_tn Char, _tn OutputIt, _tn T,
		bool check =
			std::is_enum<T>::value && !std::is_same<T, Char>::value && mapped_type_constant<T, basic_format_context<OutputIt, Char>>::value != type::custom_type,
		FMT_ENABLE_IF(check)>
		_cx auto
		write(OutputIt out, T value) -> OutputIt {
		_rt write<Char>(out, _sc<underlying_t<T>>(value));
	}
	_tp<_tn Char, _tn OutputIt, _tn T,
		FMT_ENABLE_IF(std::is_same<T, bool>::value)>
		_cx auto
		write(OutputIt out, T value, const format_specs& specs = {},
			  locale_ref = {}) -> OutputIt {
		_rt specs.type != presentation_type::none&& specs.type != presentation_type::string
			? write<Char>(out, value ? 1 : 0, specs, {})
			: write_bytes<Char>(out, value ? "true" : "false", specs);
	}
	_tp<_tn Char, _tn OutputIt> _cx auto write(OutputIt out, Char value) -> OutputIt {
		auto it = reserve(out, 1);
		*it++ = value;
		_rt base_iterator(out, it);
	}
	_tp<_tn Char, _tn OutputIt> _cx auto write(OutputIt out, const Char* value) -> OutputIt {
		if (value) _rt write(out, basic_string_view<Char>(value));
		report_error("string pointer is null");
		_rt out;
	}
	_tp<_tn Char, _tn OutputIt, _tn T,
		FMT_ENABLE_IF(std::is_same<T, void>::value)> auto
	write(OutputIt out, const T* value, const format_specs& specs = {},
		  locale_ref = {}) -> OutputIt {
		_rt write_ptr<Char>(out, bit_cast<uintptr_t>(value), &specs);
	}
	_tp<_tn Char, _tn OutputIt, _tn T,
		_tn Context = basic_format_context<OutputIt, Char>>
		_cx auto
		write(OutputIt out, const T& value) -> enable_if_t<
			std::is_class<T>::value && !has_to_string_view<T>::value && !is_floating_point<T>::value && !std::is_same<T, Char>::value && !std::is_same<T, remove_cvref_t<decltype(arg_mapper<Context>().map(value))>>::value,
			OutputIt> {
		_rt write<Char>(out, arg_mapper<Context>().map(value));
	}
	_tp<_tn Char, _tn OutputIt, _tn T,
		_tn Context = basic_format_context<OutputIt, Char>>
		_cx auto
		write(OutputIt out, const T& value)
			-> enable_if_t<mapped_type_constant<T, Context>::value == type::custom_type,
						   OutputIt> {
		auto formatter = _tn Context::_tp formatter_type<T>();
		auto parse_ctx = _tn Context::parse_context_type({});
		formatter.parse(parse_ctx);
		auto ctx = Context(out, {}, {});
		_rt formatter.format(value, ctx);
	}
	_tp<_tn Char> struct default_arg_formatter {
		using iterator = basic_appender<Char>;
		using context = buffered_context<Char>;
		iterator out;
		basic_format_args<context> args;
		locale_ref loc;
		_tp<_tn T> auto operator()(T value) -> iterator {
			_rt write<Char>(out, value);
		}
		auto operator()(_tn basic_format_arg<context>::handle h) -> iterator {
			basic_format_parse_context<Char> parse_ctx({});
			context format_ctx(out, args, loc);
			h.format(parse_ctx, format_ctx);
			_rt format_ctx.out();
		}
	};
	_tp<_tn Char> struct arg_formatter {
		using iterator = basic_appender<Char>;
		using context = buffered_context<Char>;
		iterator out;
		const format_specs& specs;
		locale_ref locale;
		_tp<_tn T> _cx FMT_INLINE auto operator()(T value) -> iterator {
			_rt detail::write<Char>(out, value, specs, locale);
		}
		auto operator()(_tn basic_format_arg<context>::handle) -> iterator {
			_rt out;
		}
	};
	struct width_checker {
		_tp<_tn T, FMT_ENABLE_IF(is_integer<T>::value)> _cx auto operator()(T value) -> ull {
			if (is_negative(value)) report_error("negative width");
			_rt _sc<ull>(value);
		}
		_tp<_tn T, FMT_ENABLE_IF(!is_integer<T>::value)> _cx auto operator()(T) -> ull {
			report_error("width is not integer");
			_rt 0;
		}
	};
	struct precision_checker {
		_tp<_tn T, FMT_ENABLE_IF(is_integer<T>::value)> _cx auto operator()(T value) -> ull {
			if (is_negative(value)) report_error("negative precision");
			_rt _sc<ull>(value);
		}
		_tp<_tn T, FMT_ENABLE_IF(!is_integer<T>::value)> _cx auto operator()(T) -> ull {
			report_error("precision is not integer");
			_rt 0;
		}
	};
	_tp<_tn Handler, _tn FormatArg> _cx auto get_dynamic_spec(FormatArg arg) -> int {
		ull value = arg.visit(Handler());
		if (value > to_unsigned(max_value<int>())) report_error("number is too big");
		_rt _sc<int>(value);
	}
	_tp<_tn Context, _tn ID> _cx auto get_arg(Context& ctx, ID id) -> decltype(ctx.arg(id)) {
		auto arg = ctx.arg(id);
		if (!arg) report_error("argument not found");
		_rt arg;
	}
	_tp<_tn Handler, _tn Context> _cx void handle_dynamic_spec(int& value,
															   arg_ref<_tn Context::char_type> ref,
															   Context& ctx) {
		switch (ref.kind) {
		case arg_id_kind::none:
			break;
		case arg_id_kind::index:
			value = detail::get_dynamic_spec<Handler>(get_arg(ctx, ref.val.index));
			break;
		case arg_id_kind::name:
			value = detail::get_dynamic_spec<Handler>(get_arg(ctx, ref.val.name));
			break;
		}
	}
#if FMT_USE_USER_DEFINED_LITERALS
#if FMT_USE_NONTYPE_TEMPLATE_ARGS
	_tp<_tn T, _tn Char, size_t N,
		fmt::detail_exported::fixed_string<Char, N> Str> struct statically_named_arg : view {
		static _cx auto name = Str.data;
		const T& value;
		statically_named_arg(const T& v)
			: value(v) {}
	};
	_tp<_tn T, _tn Char, size_t N,
		fmt::detail_exported::fixed_string<Char, N> Str> struct is_named_arg<statically_named_arg<T, Char, N, Str>> : std::true_type {};
	_tp<_tn T, _tn Char, size_t N,
		fmt::detail_exported::fixed_string<Char, N> Str> struct is_statically_named_arg<statically_named_arg<T, Char, N, Str>>
		: std::true_type {};
	_tp<_tn Char, size_t N,
		fmt::detail_exported::fixed_string<Char, N> Str> struct udl_arg {
		_tp<_tn T> auto operator=(T&& value) const {
			_rt statically_named_arg<T, Char, N, Str>(std::forward<T>(value));
		}
	};
#else
	_tp<_tn Char> struct udl_arg {
		const Char* str;
		_tp<_tn T> auto operator=(T&& value) const -> named_arg<Char, T> {
			_rt{ str, std::forward<T>(value) };
		}
	};
#endif
#endif
	_tp<_tn Locale, _tn Char> auto vformat(const Locale& loc, basic_string_view<Char> fmt,
										   _tn detail::vformat_args<Char>::type args)
		-> std::basic_string<Char> {
		auto buf = basic_memory_buffer<Char>();
		detail::vformat_to(buf, fmt, args, detail::locale_ref(loc));
		_rt{ buf.data(), buf.size() };
	}
	using format_func = void (*)(detail::buffer<char>&, int, const char*);
	FMT_API void format_error_code(buffer<char>& out, int error_code,
								   string_view message) noexcept;
	using fmt::report_error;
	FMT_API void report_error(format_func func, int error_code,
							  const char* message) noexcept;
} // namespace detail
FMT_API auto vsystem_error(int error_code, string_view format_str,
						   format_args args) -> std::system_error;
_tp<_tn... T> auto system_error(int error_code, format_string<T...> fmt, T&&... args)
	-> std::system_error {
	_rt vsystem_error(error_code, fmt, fmt::make_format_args(args...));
}
FMT_API void format_system_error(detail::buffer<char>& out, int error_code,
								 const char* message) noexcept;
FMT_API void report_system_error(int error_code, const char* message) noexcept;
class format_int {
private:
	enum { buffer_size = std::numeric_limits<ull>::digits10 + 3 };
	mutable char buffer_[buffer_size];
	char* str_;
	_tp<_tn UInt> auto format_unsigned(UInt value) -> char* {
		auto n = _sc<detail::uint32_or_64_or_128_t<UInt>>(value);
		_rt detail::format_decimal(buffer_, n, buffer_size - 1).begin;
	}
	_tp<_tn Int> auto format_signed(Int value) -> char* {
		auto abs_value = _sc<detail::uint32_or_64_or_128_t<Int>>(value);
		bool negative = value < 0;
		if (negative) abs_value = 0 - abs_value;
		auto begin = format_unsigned(abs_value);
		if (negative) *--begin = '-';
		_rt begin;
	}

public:
	explicit format_int(int value)
		: str_(format_signed(value)) {}
	explicit format_int(long value)
		: str_(format_signed(value)) {}
	explicit format_int(ll value)
		: str_(format_signed(value)) {}
	explicit format_int(unsigned value)
		: str_(format_unsigned(value)) {}
	explicit format_int(unsigned long value)
		: str_(format_unsigned(value)) {}
	explicit format_int(ull value)
		: str_(format_unsigned(value)) {}
	auto size() const -> size_t {
		_rt detail::to_unsigned(buffer_ - str_ + buffer_size - 1);
	}
	auto data() const -> const char* { _rt str_; }
	auto c_str() const -> const char* {
		buffer_[buffer_size - 1] = '\0';
		_rt str_;
	}
	auto str() const -> std::string { _rt std::string(str_, size()); }
};
_tp<_tn T, _tn Char> struct formatter<T, Char, enable_if_t<detail::has_format_as<T>::value>>
	: formatter<detail::format_as_t<T>, Char> {
	_tp<_tn FormatContext> auto format(const T& value, FormatContext& ctx) const -> decltype(ctx.out()) {
		using base = formatter<detail::format_as_t<T>, Char>;
		_rt base::format(format_as(value), ctx);
	}
};
#define FMT_FORMAT_AS(Type, Base) \
	_tp<_tn Char> struct formatter<Type, Char> : formatter<Base, Char> {}
FMT_FORMAT_AS(signed char, int);
FMT_FORMAT_AS(unsigned char, unsigned);
FMT_FORMAT_AS(short, int);
FMT_FORMAT_AS(unsigned short, unsigned);
FMT_FORMAT_AS(long, detail::long_type);
FMT_FORMAT_AS(unsigned long, detail::ulong_type);
FMT_FORMAT_AS(Char*, const Char*);
FMT_FORMAT_AS(std::basic_string<Char>, basic_string_view<Char>);
FMT_FORMAT_AS(std::nullptr_t, const void*);
FMT_FORMAT_AS(detail::std_string_view<Char>, basic_string_view<Char>);
FMT_FORMAT_AS(void*, const void*);
_tp<_tn Char, size_t N> struct formatter<Char[N], Char> : formatter<basic_string_view<Char>, Char> {};
_tp<_tn T> auto ptr(T p) -> const void* {
	static_assert(std::is_pointer<T>::value, "");
	_rt detail::bit_cast<const void*>(p);
}
_tp<_tn Enum> _cx auto underlying(Enum e) noexcept -> underlying_t<Enum> {
	_rt _sc<underlying_t<Enum>>(e);
}
namespace enums {
	_tp<_tn Enum, FMT_ENABLE_IF(std::is_enum<Enum>::value)> _cx auto format_as(Enum e) noexcept -> underlying_t<Enum> {
		_rt _sc<underlying_t<Enum>>(e);
	}
} // namespace enums
class bytes {
private:
	string_view data_;
	friend struct formatter<bytes>;

public:
	explicit bytes(string_view data)
		: data_(data) {}
};
_tp<> struct formatter<bytes> {
private:
	detail::dynamic_format_specs<> specs_;

public:
	_tp<_tn ParseContext> _cx auto parse(ParseContext& ctx) -> const char* {
		_rt parse_format_specs(ctx.begin(), ctx.end(), specs_, ctx,
							   detail::type::string_type);
	}
	_tp<_tn FormatContext> auto format(bytes b, FormatContext& ctx) const -> decltype(ctx.out()) {
		auto specs = specs_;
		detail::handle_dynamic_spec<detail::width_checker>(specs.width,
														   specs.width_ref, ctx);
		detail::handle_dynamic_spec<detail::precision_checker>(
			specs.precision, specs.precision_ref, ctx);
		_rt detail::write_bytes<char>(ctx.out(), b.data_, specs);
	}
};
_tp<_tn T> struct group_digits_view {
	T value;
};
_tp<_tn T> auto group_digits(T value) -> group_digits_view<T> {
	_rt{ value };
}
_tp<_tn T> struct formatter<group_digits_view<T>> : formatter<T> {
private:
	detail::dynamic_format_specs<> specs_;

public:
	_tp<_tn ParseContext> _cx auto parse(ParseContext& ctx) -> const char* {
		_rt parse_format_specs(ctx.begin(), ctx.end(), specs_, ctx,
							   detail::type::int_type);
	}
	_tp<_tn FormatContext> auto format(group_digits_view<T> t, FormatContext& ctx) const
		-> decltype(ctx.out()) {
		auto specs = specs_;
		detail::handle_dynamic_spec<detail::width_checker>(specs.width,
														   specs.width_ref, ctx);
		detail::handle_dynamic_spec<detail::precision_checker>(
			specs.precision, specs.precision_ref, ctx);
		_rt detail::write_int(ctx.out(),
							  _sc<detail::uint64_or_128_t<T>>(t.value),
							  0, specs, detail::digit_grouping<char>("\3", ","));
	}
};
_tp<_tn T> struct nested_view {
	const formatter<T>* fmt;
	const T* value;
};
_tp<_tn T> struct formatter<nested_view<T>> {
	_cx auto parse(format_parse_context& ctx) -> const char* {
		_rt ctx.begin();
	}
	auto format(nested_view<T> view, format_context& ctx) const
		-> decltype(ctx.out()) {
		_rt view.fmt->format(*view.value, ctx);
	}
};
_tp<_tn T> struct nested_formatter {
private:
	int width_;
	detail::fill_t fill_;
	align_t align_ : 4;
	formatter<T> formatter_;

public:
	_cx nested_formatter()
		: width_(0), align_(align_t::none) {}
	_cx auto parse(format_parse_context& ctx) -> const char* {
		auto specs = detail::dynamic_format_specs<char>();
		auto it = parse_format_specs(ctx.begin(), ctx.end(), specs, ctx,
									 detail::type::none_type);
		width_ = specs.width;
		fill_ = specs.fill;
		align_ = specs.align;
		ctx.advance_to(it);
		_rt formatter_.parse(ctx);
	}
	_tp<_tn F> auto write_padded(format_context& ctx, F write) const -> decltype(ctx.out()) {
		if (width_ == 0) _rt write(ctx.out());
		auto buf = memory_buffer();
		write(appender(buf));
		auto specs = format_specs();
		specs.width = width_;
		specs.fill = fill_;
		specs.align = align_;
		_rt detail::write<char>(ctx.out(), string_view(buf.data(), buf.size()),
								specs);
	}
	auto nested(const T& value) const -> nested_view<T> {
		_rt nested_view<T>{ &formatter_, &value };
	}
};
_tp<_tn T, FMT_ENABLE_IF(!std::is_integral<T>::value && !detail::has_format_as<T>::value)> inline auto to_string(const T& value) -> std::string {
	auto buffer = memory_buffer();
	detail::write<char>(appender(buffer), value);
	_rt{ buffer.data(), buffer.size() };
}
_tp<_tn T, FMT_ENABLE_IF(std::is_integral<T>::value)>
	FMT_NODISCARD inline auto to_string(T value) -> std::string {
	_cx int max_size = detail::digits10<T>() + 2;
	char buffer[max_size > 5 ? _sc<unsigned>(max_size) : 5];
	char* begin = buffer;
	_rt std::string(begin, detail::write<char>(begin, value));
}
_tp<_tn Char, size_t SIZE>
	FMT_NODISCARD auto to_string(const basic_memory_buffer<Char, SIZE>& buf)
		-> std::basic_string<Char> {
	auto size = buf.size();
	detail::assume(size < std::basic_string<Char>().max_size());
	_rt std::basic_string<Char>(buf.data(), size);
}
_tp<_tn T, FMT_ENABLE_IF(!std::is_integral<T>::value && detail::has_format_as<T>::value)> inline auto to_string(const T& value) -> std::string {
	_rt to_string(format_as(value));
}
FMT_END_EXPORT
namespace detail {
	_tp<_tn Char> void vformat_to(buffer<Char>& buf, basic_string_view<Char> fmt,
								  _tn vformat_args<Char>::type args, locale_ref loc) {
		auto out = basic_appender<Char>(buf);
		if (fmt.size() == 2 && equal2(fmt.data(), "{}")) {
			auto arg = args.get(0);
			if (!arg) report_error("argument not found");
			arg.visit(default_arg_formatter<Char>{ out, args, loc });
			_rt;
		}
		struct format_handler {
			basic_format_parse_context<Char> parse_context;
			buffered_context<Char> context;
			format_handler(basic_appender<Char> p_out, basic_string_view<Char> str,
						   basic_format_args<buffered_context<Char>> p_args,
						   locale_ref p_loc)
				: parse_context(str), context(p_out, p_args, p_loc) {}
			void on_text(const Char* begin, const Char* end) {
				auto text = basic_string_view<Char>(begin, to_unsigned(end - begin));
				context.advance_to(write<Char>(context.out(), text));
			}
			_cx auto on_arg_id() -> int {
				_rt parse_context.next_arg_id();
			}
			_cx auto on_arg_id(int id) -> int {
				parse_context.check_arg_id(id);
				_rt id;
			}
			_cx auto on_arg_id(basic_string_view<Char> id) -> int {
				parse_context.check_arg_id(id);
				int arg_id = context.arg_id(id);
				if (arg_id < 0) report_error("argument not found");
				_rt arg_id;
			}
			FMT_INLINE void on_replacement_field(int id, const Char*) {
				auto arg = get_arg(context, id);
				context.advance_to(arg.visit(default_arg_formatter<Char>{
					context.out(), context.args(), context.locale() }));
			}
			auto on_format_specs(int id, const Char* begin, const Char* end)
				-> const Char* {
				auto arg = get_arg(context, id);
				if (arg.format_custom(begin, parse_context, context))
					_rt parse_context.begin();
				auto specs = detail::dynamic_format_specs<Char>();
				begin = parse_format_specs(begin, end, specs, parse_context, arg.type());
				detail::handle_dynamic_spec<detail::width_checker>(
					specs.width, specs.width_ref, context);
				detail::handle_dynamic_spec<detail::precision_checker>(
					specs.precision, specs.precision_ref, context);
				if (begin == end || *begin != '}')
					report_error("missing '}' in format string");
				context.advance_to(arg.visit(
					arg_formatter<Char>{ context.out(), specs, context.locale() }));
				_rt begin;
			}
			void on_error(const char* message) { report_error(message); }
		};
		detail::parse_format_string<false>(fmt, format_handler(out, fmt, args, loc));
	}
	FMT_BEGIN_EXPORT
#ifndef FMT_HEADER_ONLY
	extern _tp FMT_API void vformat_to(buffer<char>&, string_view,
									   _tn vformat_args<>::type,
									   locale_ref);
	extern _tp FMT_API auto thousands_sep_impl<char>(locale_ref)
		-> thousands_sep_result<char>;
	extern _tp FMT_API auto thousands_sep_impl<wchar_t>(locale_ref)
		-> thousands_sep_result<wchar_t>;
	extern _tp FMT_API auto decimal_point_impl(locale_ref) -> char;
	extern _tp FMT_API auto decimal_point_impl(locale_ref) -> wchar_t;
#endif
} // namespace detail
#if FMT_USE_USER_DEFINED_LITERALS
inline namespace literals {
#if FMT_USE_NONTYPE_TEMPLATE_ARGS
	_tp<detail_exported::fixed_string Str> _cx auto operator""_a() {
		using char_t = remove_cvref_t<decltype(Str.data[0])>;
		_rt detail::udl_arg<char_t, sizeof(Str.data) / sizeof(char_t), Str>();
	}
#else
	_cx auto operator""_a(const char* s, size_t) -> detail::udl_arg<char> {
		_rt{ s };
	}
#endif
} // namespace literals
#endif
FMT_API auto vformat(string_view fmt, format_args args) -> std::string;
_tp<_tn... T>
	FMT_NODISCARD FMT_INLINE auto format(format_string<T...> fmt, T&&... args)
		-> std::string {
	_rt vformat(fmt, fmt::make_format_args(args...));
}
_tp<_tn Locale, FMT_ENABLE_IF(detail::is_locale<Locale>::value)> inline auto vformat(const Locale& loc, string_view fmt, format_args args)
	-> std::string {
	_rt detail::vformat(loc, fmt, args);
}
_tp<_tn Locale, _tn... T,
	FMT_ENABLE_IF(detail::is_locale<Locale>::value)> inline auto
format(const Locale& loc, format_string<T...> fmt, T&&... args)
	-> std::string {
	_rt fmt::vformat(loc, string_view(fmt), fmt::make_format_args(args...));
}
_tp<_tn OutputIt, _tn Locale,
	FMT_ENABLE_IF(detail::is_output_iterator<OutputIt, char>::value&&
					  detail::is_locale<Locale>::value)> auto
vformat_to(OutputIt out, const Locale& loc, string_view fmt,
		   format_args args) -> OutputIt {
	using detail::get_buffer;
	auto&& buf = get_buffer<char>(out);
	detail::vformat_to(buf, fmt, args, detail::locale_ref(loc));
	_rt detail::get_iterator(buf, out);
}
_tp<_tn OutputIt, _tn Locale, _tn... T,
	FMT_ENABLE_IF(detail::is_output_iterator<OutputIt, char>::value&&
					  detail::is_locale<Locale>::value)>
	FMT_INLINE auto format_to(OutputIt out, const Locale& loc,
							  format_string<T...> fmt, T&&... args) -> OutputIt {
	_rt vformat_to(out, loc, fmt, fmt::make_format_args(args...));
}
_tp<_tn Locale, _tn... T,
	FMT_ENABLE_IF(detail::is_locale<Locale>::value)>
	FMT_NODISCARD FMT_INLINE auto formatted_size(const Locale& loc,
												 format_string<T...> fmt,
												 T&&... args) -> size_t {
	auto buf = detail::counting_buffer<>();
	detail::vformat_to<char>(buf, fmt, fmt::make_format_args(args...),
							 detail::locale_ref(loc));
	_rt buf.count();
}
FMT_END_EXPORT
_tp<_tn T, _tn Char>
	_tp<_tn FormatContext> _cx FMT_INLINE auto
	formatter<T, Char,
			  enable_if_t<detail::type_constant<T, Char>::value != detail::type::custom_type>>::format(const T& val,
																									   FormatContext& ctx)
		const -> decltype(ctx.out()) {
	if (specs_.width_ref.kind == detail::arg_id_kind::none && specs_.precision_ref.kind == detail::arg_id_kind::none) {
		_rt detail::write<Char>(ctx.out(), val, specs_, ctx.locale());
	}
	auto specs = specs_;
	detail::handle_dynamic_spec<detail::width_checker>(specs.width,
													   specs.width_ref, ctx);
	detail::handle_dynamic_spec<detail::precision_checker>(
		specs.precision, specs.precision_ref, ctx);
	_rt detail::write<Char>(ctx.out(), val, specs, ctx.locale());
}
FMT_END_NAMESPACE
#ifdef FMT_REMOVE_TRANSITIVE_INCLUDES
#undef _LIBCPP_REMOVE_TRANSITIVE_INCLUDES
#endif
#endif
#define FMT_FUNC inline
#ifndef FMT_FORMAT_INL_H_
#define FMT_FORMAT_INL_H_
#include <algorithm>
#include <cerrno>
#include <climits>
#include <cmath>
#include <exception>
#ifndef FMT_STATIC_THOUSANDS_SEPARATOR
#include <locale>
#endif
#if defined(_WIN32) && !defined(FMT_WINDOWS_NO_WCHAR)
#include <io.h>
#endif
FMT_BEGIN_NAMESPACE
namespace detail {
	FMT_FUNC void assert_fail(const char* file, int line, const char* message) {
		std::fprintf(stderr, "%s:%d: assertion failed: %s", file, line, message);
		std::terminate();
	}
	FMT_FUNC void format_error_code(detail::buffer<char>& out, int error_code,
									string_view message) noexcept {
		out.try_resize(0);
		static const char SEP[] = ": ";
		static const char ERROR_STR[] = "error ";
		size_t error_code_size = sizeof(SEP) + sizeof(ERROR_STR) - 2;
		auto abs_value = _sc<uint32_or_64_or_128_t<int>>(error_code);
		if (detail::is_negative(error_code)) {
			abs_value = 0 - abs_value;
			++error_code_size;
		}
		error_code_size += detail::to_unsigned(detail::count_digits(abs_value));
		auto it = appender(out);
		if (message.size() <= inline_buffer_size - error_code_size)
			fmt::format_to(it, FMT_STRING("{}{}"), message, SEP);
		fmt::format_to(it, FMT_STRING("{}{}"), ERROR_STR, error_code);
		FMT_ASSERT(out.size() <= inline_buffer_size, "");
	}
	FMT_FUNC void report_error(format_func func, int error_code,
							   const char* message) noexcept {
		memory_buffer full_message;
		func(full_message, error_code, message);
		if (std::fwrite(full_message.data(), full_message.size(), 1, stderr) > 0)
			std::fputc('\n', stderr);
	}
	inline void fwrite_fully(const void* ptr, size_t count, FILE* stream) {
		size_t written = std::fwrite(ptr, 1, count, stream);
		if (written < count)
			FMT_THROW(system_error(errno, FMT_STRING("cannot write to file")));
	}
#ifndef FMT_STATIC_THOUSANDS_SEPARATOR
	_tp<_tn Locale>
	locale_ref::locale_ref(const Locale& loc)
		: locale_(&loc) {
		static_assert(std::is_same<Locale, std::locale>::value, "");
	}
	_tp<_tn Locale> auto locale_ref::get() const -> Locale {
		static_assert(std::is_same<Locale, std::locale>::value, "");
		_rt locale_ ? *_sc<const std::locale*>(locale_) : std::locale();
	}
	_tp<_tn Char>
		FMT_FUNC auto thousands_sep_impl(locale_ref loc) -> thousands_sep_result<Char> {
		auto& facet = std::use_facet<std::numpunct<Char>>(loc.get<std::locale>());
		auto grouping = facet.grouping();
		auto thousands_sep = grouping.empty() ? Char() : facet.thousands_sep();
		_rt{ std::move(grouping), thousands_sep };
	}
	_tp<_tn Char>
		FMT_FUNC auto decimal_point_impl(locale_ref loc) -> Char {
		_rt std::use_facet<std::numpunct<Char>>(loc.get<std::locale>())
			.decimal_point();
	}
#else
	_tp<_tn Char>
		FMT_FUNC auto thousands_sep_impl(locale_ref) -> thousands_sep_result<Char> {
		_rt{ "\03", FMT_STATIC_THOUSANDS_SEPARATOR };
	}
	_tp<_tn Char>
		FMT_FUNC Char decimal_point_impl(locale_ref) {
		_rt '.';
	}
#endif
	FMT_FUNC auto write_loc(appender out, loc_value value,
							const format_specs& specs, locale_ref loc) -> bool {
#ifndef FMT_STATIC_THOUSANDS_SEPARATOR
		auto locale = loc.get<std::locale>();
		using facet = format_facet<std::locale>;
		if (std::has_facet<facet>(locale))
			_rt std::use_facet<facet>(locale).put(out, value, specs);
		_rt facet(locale).put(out, value, specs);
#endif
		_rt false;
	}
} // namespace detail
FMT_FUNC void report_error(const char* message) {
	FMT_THROW(format_error(message));
}
_tp<_tn Locale>
	_tn Locale::id format_facet<Locale>::id;
#ifndef FMT_STATIC_THOUSANDS_SEPARATOR
_tp<_tn Locale>
format_facet<Locale>::format_facet(Locale& loc) {
	auto& numpunct = std::use_facet<std::numpunct<char>>(loc);
	grouping_ = numpunct.grouping();
	if (!grouping_.empty()) separator_ = std::string(1, numpunct.thousands_sep());
}
_tp<>
	FMT_API FMT_FUNC auto format_facet<std::locale>::do_put(
		appender out, loc_value val, const format_specs& specs) const -> bool {
	_rt val.visit(
		detail::loc_writer<>{ out, specs, separator_, grouping_, decimal_point_ });
}
#endif
FMT_FUNC auto vsystem_error(int error_code, string_view fmt, format_args args)
	-> std::system_error {
	auto ec = std::error_code(error_code, std::generic_category());
	_rt std::system_error(ec, vformat(fmt, args));
}
namespace detail {
	_tp<_tn F> inline auto operator==(basic_fp<F> x, basic_fp<F> y) -> bool {
		_rt x.f == y.f&& x.e == y.e;
	}
	_cx inline auto rotr(uint32_t n, uint32_t r) noexcept -> uint32_t {
		r &= 31;
		_rt(n >> r) | (n << (32 - r));
	}
	_cx inline auto rotr(uint64_t n, uint32_t r) noexcept -> uint64_t {
		r &= 63;
		_rt(n >> r) | (n << (64 - r));
	}
	namespace dragonbox {
		inline auto umul96_upper64(uint32_t x, uint64_t y) noexcept -> uint64_t {
			_rt umul128_upper64(_sc<uint64_t>(x) << 32, y);
		}
		inline auto umul192_lower128(uint64_t x, uint128_fallback y) noexcept
			-> uint128_fallback {
			uint64_t high = x * y.high();
			uint128_fallback high_low = umul128(x, y.low());
			_rt{ high + high_low.high(), high_low.low() };
		}
		inline auto umul96_lower64(uint32_t x, uint64_t y) noexcept -> uint64_t {
			_rt x* y;
		}
		inline auto floor_log10_pow2_minus_log10_4_over_3(int e) noexcept -> int {
			FMT_ASSERT(e <= 2936 && e >= -2985, "too large exponent");
			_rt(e * 631305 - 261663) >> 21;
		}
		FMT_INLINE_VARIABLE _cx struct {
			uint32_t divisor;
			int shift_amount;
		} div_small_pow10_infos[] = { { 10, 16 }, { 100, 16 } };
		_tp<int N> auto check_divisibility_and_divide_by_pow10(uint32_t& n) noexcept -> bool {
			_cx auto info = div_small_pow10_infos[N - 1];
			FMT_ASSERT(n <= info.divisor * 10, "n is too large");
			_cx uint32_t magic_number =
				(1u << info.shift_amount) / info.divisor + 1;
			n *= magic_number;
			const uint32_t comparison_mask = (1u << info.shift_amount) - 1;
			bool result = (n & comparison_mask) < magic_number;
			n >>= info.shift_amount;
			_rt result;
		}
		_tp<int N> auto small_division_by_pow10(uint32_t n) noexcept -> uint32_t {
			_cx auto info = div_small_pow10_infos[N - 1];
			FMT_ASSERT(n <= info.divisor * 10, "n is too large");
			_cx uint32_t magic_number =
				(1u << info.shift_amount) / info.divisor + 1;
			_rt(n * magic_number) >> info.shift_amount;
		}
		inline auto divide_by_10_to_kappa_plus_1(uint32_t n) noexcept -> uint32_t {
			_rt _sc<uint32_t>((_sc<uint64_t>(n) * 1374389535) >> 37);
		}
		inline auto divide_by_10_to_kappa_plus_1(uint64_t n) noexcept -> uint64_t {
			_rt umul128_upper64(n, 2361183241434822607ull) >> 7;
		}
		_tp<_tn T> struct cache_accessor;
		_tp<> struct cache_accessor<float> {
			using carrier_uint = float_info<float>::carrier_uint;
			using cache_entry_type = uint64_t;
			static auto get_cached_power(int k) noexcept -> uint64_t {
				FMT_ASSERT(k >= float_info<float>::min_k && k <= float_info<float>::max_k,
						   "k is out of range");
				static _cx const uint64_t pow10_significands[] = {
					0x81ceb32c4b43fcf5, 0xa2425ff75e14fc32, 0xcad2f7f5359a3b3f,
					0xfd87b5f28300ca0e, 0x9e74d1b791e07e49, 0xc612062576589ddb,
					0xf79687aed3eec552, 0x9abe14cd44753b53, 0xc16d9a0095928a28,
					0xf1c90080baf72cb2, 0x971da05074da7bef, 0xbce5086492111aeb,
					0xec1e4a7db69561a6, 0x9392ee8e921d5d08, 0xb877aa3236a4b44a,
					0xe69594bec44de15c, 0x901d7cf73ab0acda, 0xb424dc35095cd810,
					0xe12e13424bb40e14, 0x8cbccc096f5088cc, 0xafebff0bcb24aaff,
					0xdbe6fecebdedd5bf, 0x89705f4136b4a598, 0xabcc77118461cefd,
					0xd6bf94d5e57a42bd, 0x8637bd05af6c69b6, 0xa7c5ac471b478424,
					0xd1b71758e219652c, 0x83126e978d4fdf3c, 0xa3d70a3d70a3d70b,
					0xcccccccccccccccd, 0x8000000000000000, 0xa000000000000000,
					0xc800000000000000, 0xfa00000000000000, 0x9c40000000000000,
					0xc350000000000000, 0xf424000000000000, 0x9896800000000000,
					0xbebc200000000000, 0xee6b280000000000, 0x9502f90000000000,
					0xba43b74000000000, 0xe8d4a51000000000, 0x9184e72a00000000,
					0xb5e620f480000000, 0xe35fa931a0000000, 0x8e1bc9bf04000000,
					0xb1a2bc2ec5000000, 0xde0b6b3a76400000, 0x8ac7230489e80000,
					0xad78ebc5ac620000, 0xd8d726b7177a8000, 0x878678326eac9000,
					0xa968163f0a57b400, 0xd3c21bcecceda100, 0x84595161401484a0,
					0xa56fa5b99019a5c8, 0xcecb8f27f4200f3a, 0x813f3978f8940985,
					0xa18f07d736b90be6, 0xc9f2c9cd04674edf, 0xfc6f7c4045812297,
					0x9dc5ada82b70b59e, 0xc5371912364ce306, 0xf684df56c3e01bc7,
					0x9a130b963a6c115d, 0xc097ce7bc90715b4, 0xf0bdc21abb48db21,
					0x96769950b50d88f5, 0xbc143fa4e250eb32, 0xeb194f8e1ae525fe,
					0x92efd1b8d0cf37bf, 0xb7abc627050305ae, 0xe596b7b0c643c71a,
					0x8f7e32ce7bea5c70, 0xb35dbf821ae4f38c, 0xe0352f62a19e306f
				};
				_rt pow10_significands[k - float_info<float>::min_k];
			}
			struct compute_mul_result {
				carrier_uint result;
				bool is_integer;
			};
			struct compute_mul_parity_result {
				bool parity;
				bool is_integer;
			};
			static auto compute_mul(carrier_uint u,
									const cache_entry_type& cache) noexcept
				-> compute_mul_result {
				auto r = umul96_upper64(u, cache);
				_rt{ _sc<carrier_uint>(r >> 32),
					 _sc<carrier_uint>(r) == 0 };
			}
			static auto compute_delta(const cache_entry_type& cache, int beta) noexcept
				-> uint32_t {
				_rt _sc<uint32_t>(cache >> (64 - 1 - beta));
			}
			static auto compute_mul_parity(carrier_uint two_f,
										   const cache_entry_type& cache,
										   int beta) noexcept
				-> compute_mul_parity_result {
				FMT_ASSERT(beta >= 1, "");
				FMT_ASSERT(beta < 64, "");
				auto r = umul96_lower64(two_f, cache);
				_rt{ ((r >> (64 - beta)) & 1) != 0,
					 _sc<uint32_t>(r >> (32 - beta)) == 0 };
			}
			static auto compute_left_endpoint_for_shorter_interval_case(
				const cache_entry_type& cache, int beta) noexcept -> carrier_uint {
				_rt _sc<carrier_uint>(
					(cache - (cache >> (num_significand_bits<float>() + 2))) >> (64 - num_significand_bits<float>() - 1 - beta));
			}
			static auto compute_right_endpoint_for_shorter_interval_case(
				const cache_entry_type& cache, int beta) noexcept -> carrier_uint {
				_rt _sc<carrier_uint>(
					(cache + (cache >> (num_significand_bits<float>() + 1))) >> (64 - num_significand_bits<float>() - 1 - beta));
			}
			static auto compute_round_up_for_shorter_interval_case(
				const cache_entry_type& cache, int beta) noexcept -> carrier_uint {
				_rt(_sc<carrier_uint>(
						cache >> (64 - num_significand_bits<float>() - 2 - beta))
					+ 1)
					/ 2;
			}
		};
		_tp<> struct cache_accessor<double> {
			using carrier_uint = float_info<double>::carrier_uint;
			using cache_entry_type = uint128_fallback;
			static auto get_cached_power(int k) noexcept -> uint128_fallback {
				FMT_ASSERT(k >= float_info<double>::min_k && k <= float_info<double>::max_k,
						   "k is out of range");
				static _cx const uint128_fallback pow10_significands[] = {
					{ 0xff77b1fcbebcdc4f, 0x25e8e89c13bb0f7b },
					{ 0xce5d73ff402d98e3, 0xfb0a3d212dc81290 },
					{ 0xa6b34ad8c9dfc06f, 0xf42faa48c0ea481f },
					{ 0x86a8d39ef77164bc, 0xae5dff9c02033198 },
					{ 0xd98ddaee19068c76, 0x3badd624dd9b0958 },
					{ 0xafbd2350644eeacf, 0xe5d1929ef90898fb },
					{ 0x8df5efabc5979c8f, 0xca8d3ffa1ef463c2 },
					{ 0xe55990879ddcaabd, 0xcc420a6a101d0516 },
					{ 0xb94470938fa89bce, 0xf808e40e8d5b3e6a },
					{ 0x95a8637627989aad, 0xdde7001379a44aa9 },
					{ 0xf1c90080baf72cb1, 0x5324c68b12dd6339 },
					{ 0xc350000000000000, 0x0000000000000000 },
					{ 0x9dc5ada82b70b59d, 0xf020000000000000 },
					{ 0xfee50b7025c36a08, 0x02f236d04753d5b5 },
					{ 0xcde6fd5e09abcf26, 0xed4c0226b55e6f87 },
					{ 0xa6539930bf6bff45, 0x84db8346b786151d },
					{ 0x865b86925b9bc5c2, 0x0b8a2392ba45a9b3 },
					{ 0xd910f7ff28069da4, 0x1b2ba1518094da05 },
					{ 0xaf58416654a6babb, 0x387ac8d1970027b3 },
					{ 0x8da471a9de737e24, 0x5ceaecfed289e5d3 },
					{ 0xe4d5e82392a40515, 0x0fabaf3feaa5334b },
					{ 0xb8da1662e7b00a17, 0x3d6a751f3b936244 },
					{ 0x95527a5202df0ccb, 0x0f37801e0c43ebc9 },
					{ 0xf13e34aabb430a15,
					  0x647726b9e7c68ff0 }
				};
				static _cx const uint64_t powers_of_5_64[] = {
					0x0000000000000001, 0x0000000000000005, 0x0000000000000019,
					0x000000000000007d, 0x0000000000000271, 0x0000000000000c35,
					0x0000000000003d09, 0x000000000001312d, 0x000000000005f5e1,
					0x00000000001dcd65, 0x00000000009502f9, 0x0000000002e90edd,
					0x000000000e8d4a51, 0x0000000048c27395, 0x000000016bcc41e9,
					0x000000071afd498d, 0x0000002386f26fc1, 0x000000b1a2bc2ec5,
					0x000003782dace9d9, 0x00001158e460913d, 0x000056bc75e2d631,
					0x0001b1ae4d6e2ef5, 0x000878678326eac9, 0x002a5a058fc295ed,
					0x00d3c21bcecceda1, 0x0422ca8b0a00a425, 0x14adf4b7320334b9
				};
				static const int compression_ratio = 27;
				int cache_index = (k - float_info<double>::min_k) / compression_ratio;
				int kb = cache_index * compression_ratio + float_info<double>::min_k;
				int offset = k - kb;
				uint128_fallback base_cache = pow10_significands[cache_index];
				if (offset == 0) _rt base_cache;
				int alpha = floor_log2_pow10(kb + offset) - floor_log2_pow10(kb) - offset;
				FMT_ASSERT(alpha > 0 && alpha < 64, "shifting error detected");
				uint64_t pow5 = powers_of_5_64[offset];
				uint128_fallback recovered_cache = umul128(base_cache.high(), pow5);
				uint128_fallback middle_low = umul128(base_cache.low(), pow5);
				recovered_cache += middle_low.high();
				uint64_t high_to_middle = recovered_cache.high() << (64 - alpha);
				uint64_t middle_to_low = recovered_cache.low() << (64 - alpha);
				recovered_cache =
					uint128_fallback{ (recovered_cache.low() >> alpha) | high_to_middle,
									  ((middle_low.low() >> alpha) | middle_to_low) };
				FMT_ASSERT(recovered_cache.low() + 1 != 0, "");
				_rt{ recovered_cache.high(), recovered_cache.low() + 1 };
			}
			struct compute_mul_result {
				carrier_uint result;
				bool is_integer;
			};
			struct compute_mul_parity_result {
				bool parity;
				bool is_integer;
			};
			static auto compute_mul(carrier_uint u,
									const cache_entry_type& cache) noexcept
				-> compute_mul_result {
				auto r = umul192_upper128(u, cache);
				_rt{ r.high(), r.low() == 0 };
			}
			static auto compute_delta(cache_entry_type const& cache, int beta) noexcept
				-> uint32_t {
				_rt _sc<uint32_t>(cache.high() >> (64 - 1 - beta));
			}
			static auto compute_mul_parity(carrier_uint two_f,
										   const cache_entry_type& cache,
										   int beta) noexcept
				-> compute_mul_parity_result {
				FMT_ASSERT(beta >= 1, "");
				FMT_ASSERT(beta < 64, "");
				auto r = umul192_lower128(two_f, cache);
				_rt{ ((r.high() >> (64 - beta)) & 1) != 0,
					 ((r.high() << beta) | (r.low() >> (64 - beta))) == 0 };
			}
			static auto compute_left_endpoint_for_shorter_interval_case(
				const cache_entry_type& cache, int beta) noexcept -> carrier_uint {
				_rt(cache.high() - (cache.high() >> (num_significand_bits<double>() + 2))) >> (64 - num_significand_bits<double>() - 1 - beta);
			}
			static auto compute_right_endpoint_for_shorter_interval_case(
				const cache_entry_type& cache, int beta) noexcept -> carrier_uint {
				_rt(cache.high() + (cache.high() >> (num_significand_bits<double>() + 1))) >> (64 - num_significand_bits<double>() - 1 - beta);
			}
			static auto compute_round_up_for_shorter_interval_case(
				const cache_entry_type& cache, int beta) noexcept -> carrier_uint {
				_rt((cache.high() >> (64 - num_significand_bits<double>() - 2 - beta)) + 1) / 2;
			}
		};
		FMT_FUNC auto get_cached_power(int k) noexcept -> uint128_fallback {
			_rt cache_accessor<double>::get_cached_power(k);
		}
		_tp<_tn T> auto is_left_endpoint_integer_shorter_interval(int exponent) noexcept -> bool {
			const int case_shorter_interval_left_endpoint_lower_threshold = 2;
			const int case_shorter_interval_left_endpoint_upper_threshold = 3;
			_rt exponent >= case_shorter_interval_left_endpoint_lower_threshold&& exponent <= case_shorter_interval_left_endpoint_upper_threshold;
		}
		FMT_INLINE int remove_trailing_zeros(uint32_t& n, int s = 0) noexcept {
			FMT_ASSERT(n != 0, "");
			_cx uint32_t mod_inv_5 = 0xcccccccd;
			_cx uint32_t mod_inv_25 = 0xc28f5c29;
			while (true) {
				auto q = rotr(n * mod_inv_25, 2);
				if (q > max_value<uint32_t>() / 100) break;
				n = q;
				s += 2;
			}
			auto q = rotr(n * mod_inv_5, 1);
			if (q <= max_value<uint32_t>() / 10) {
				n = q;
				s |= 1;
			}
			_rt s;
		}
		FMT_INLINE int remove_trailing_zeros(uint64_t& n) noexcept {
			FMT_ASSERT(n != 0, "");
			_cx uint64_t magic_number = 12379400392853802749ull;
			auto nm = umul128(n, magic_number);
			if ((nm.high() & ((1ull << (90 - 64)) - 1)) == 0 && nm.low() < magic_number) {
				auto n32 = _sc<uint32_t>(nm.high() >> (90 - 64));
				int s = remove_trailing_zeros(n32, 8);
				n = n32;
				_rt s;
			}
			_cx uint64_t mod_inv_5 = 0xcccccccccccccccd;
			_cx uint64_t mod_inv_25 = 0x8f5c28f5c28f5c29;
			int s = 0;
			while (true) {
				auto q = rotr(n * mod_inv_25, 2);
				if (q > max_value<uint64_t>() / 100) break;
				n = q;
				s += 2;
			}
			auto q = rotr(n * mod_inv_5, 1);
			if (q <= max_value<uint64_t>() / 10) {
				n = q;
				s |= 1;
			}
			_rt s;
		}
		_tp<_tn T>
			FMT_INLINE decimal_fp<T> shorter_interval_case(int exponent) noexcept {
			decimal_fp<T> ret_value;
			const int minus_k = floor_log10_pow2_minus_log10_4_over_3(exponent);
			const int beta = exponent + floor_log2_pow10(-minus_k);
			using cache_entry_type = _tn cache_accessor<T>::cache_entry_type;
			const cache_entry_type cache = cache_accessor<T>::get_cached_power(-minus_k);
			auto xi = cache_accessor<T>::compute_left_endpoint_for_shorter_interval_case(
				cache, beta);
			auto zi = cache_accessor<T>::compute_right_endpoint_for_shorter_interval_case(
				cache, beta);
			if (!is_left_endpoint_integer_shorter_interval<T>(exponent)) ++xi;
			ret_value.significand = zi / 10;
			if (ret_value.significand * 10 >= xi) {
				ret_value.exponent = minus_k + 1;
				ret_value.exponent += remove_trailing_zeros(ret_value.significand);
				_rt ret_value;
			}
			ret_value.significand =
				cache_accessor<T>::compute_round_up_for_shorter_interval_case(cache,
																			  beta);
			ret_value.exponent = minus_k;
			if (exponent >= float_info<T>::shorter_interval_tie_lower_threshold && exponent <= float_info<T>::shorter_interval_tie_upper_threshold) {
				ret_value.significand = ret_value.significand % 2 == 0
											? ret_value.significand
											: ret_value.significand - 1;
			}
			else if (ret_value.significand < xi) {
				++ret_value.significand;
			}
			_rt ret_value;
		}
		_tp<_tn T> auto to_decimal(T x) noexcept -> decimal_fp<T> {
			using carrier_uint = _tn float_info<T>::carrier_uint;
			using cache_entry_type = _tn cache_accessor<T>::cache_entry_type;
			auto br = bit_cast<carrier_uint>(x);
			const carrier_uint significand_mask =
				(_sc<carrier_uint>(1) << num_significand_bits<T>()) - 1;
			carrier_uint significand = (br & significand_mask);
			int exponent =
				_sc<int>((br & exponent_mask<T>()) >> num_significand_bits<T>());
			if (exponent != 0) {
				exponent -= exponent_bias<T>() + num_significand_bits<T>();
				if (significand == 0) _rt shorter_interval_case<T>(exponent);
				significand |= (_sc<carrier_uint>(1) << num_significand_bits<T>());
			}
			else {
				if (significand == 0) _rt{ 0, 0 };
				exponent =
					std::numeric_limits<T>::min_exponent - num_significand_bits<T>() - 1;
			}
			const bool include_left_endpoint = (significand % 2 == 0);
			const bool include_right_endpoint = include_left_endpoint;
			const int minus_k = floor_log10_pow2(exponent) - float_info<T>::kappa;
			const cache_entry_type cache = cache_accessor<T>::get_cached_power(-minus_k);
			const int beta = exponent + floor_log2_pow10(-minus_k);
			const uint32_t deltai = cache_accessor<T>::compute_delta(cache, beta);
			const carrier_uint two_fc = significand << 1;
			const _tn cache_accessor<T>::compute_mul_result z_mul =
				cache_accessor<T>::compute_mul((two_fc | 1) << beta, cache);
			decimal_fp<T> ret_value;
			ret_value.significand = divide_by_10_to_kappa_plus_1(z_mul.result);
			uint32_t r = _sc<uint32_t>(z_mul.result - float_info<T>::big_divisor * ret_value.significand);
			if (r < deltai) {
				if (r == 0 && (z_mul.is_integer & !include_right_endpoint)) {
					--ret_value.significand;
					r = float_info<T>::big_divisor;
					goto small_divisor_case_label;
				}
			}
			else if (r > deltai) {
				goto small_divisor_case_label;
			}
			else {
				const _tn cache_accessor<T>::compute_mul_parity_result x_mul =
					cache_accessor<T>::compute_mul_parity(two_fc - 1, cache, beta);
				if (!(x_mul.parity | (x_mul.is_integer & include_left_endpoint)))
					goto small_divisor_case_label;
			}
			ret_value.exponent = minus_k + float_info<T>::kappa + 1;
			ret_value.exponent += remove_trailing_zeros(ret_value.significand);
			_rt ret_value;
		small_divisor_case_label:
			ret_value.significand *= 10;
			ret_value.exponent = minus_k + float_info<T>::kappa;
			uint32_t dist = r - (deltai / 2) + (float_info<T>::small_divisor / 2);
			const bool approx_y_parity =
				((dist ^ (float_info<T>::small_divisor / 2)) & 1) != 0;
			const bool divisible_by_small_divisor =
				check_divisibility_and_divide_by_pow10<float_info<T>::kappa>(dist);
			ret_value.significand += dist;
			if (!divisible_by_small_divisor) _rt ret_value;
			const auto y_mul = cache_accessor<T>::compute_mul_parity(two_fc, cache, beta);
			if (y_mul.parity != approx_y_parity)
				--ret_value.significand;
			else if (y_mul.is_integer & (ret_value.significand % 2 != 0))
				--ret_value.significand;
			_rt ret_value;
		}
	} // namespace dragonbox
} // namespace detail
_tp<> struct formatter<detail::bigint> {
	_cx auto parse(format_parse_context& ctx)
		-> format_parse_context::iterator {
		_rt ctx.begin();
	}
	auto format(const detail::bigint& n, format_context& ctx) const
		-> format_context::iterator {
		auto out = ctx.out();
		bool first = true;
		for (auto i = n.bigits_.size(); i > 0; --i) {
			auto value = n.bigits_[i - 1u];
			if (first) {
				out = fmt::format_to(out, FMT_STRING("{:x}"), value);
				first = false;
				continue;
			}
			out = fmt::format_to(out, FMT_STRING("{:08x}"), value);
		}
		if (n.exp_ > 0)
			out = fmt::format_to(out, FMT_STRING("p{}"),
								 n.exp_ * detail::bigint::bigit_bits);
		_rt out;
	}
};
FMT_FUNC detail::utf8_to_utf16::utf8_to_utf16(string_view s) {
	for_each_codepoint(s, [this](uint32_t cp, string_view) {
		if (cp == invalid_code_point) FMT_THROW(std::runtime_error("invalid utf8"));
		if (cp <= 0xFFFF) {
			buffer_.push_back(_sc<wchar_t>(cp));
		}
		else {
			cp -= 0x10000;
			buffer_.push_back(_sc<wchar_t>(0xD800 + (cp >> 10)));
			buffer_.push_back(_sc<wchar_t>(0xDC00 + (cp & 0x3FF)));
		}
		_rt true;
	});
	buffer_.push_back(0);
}
FMT_FUNC void format_system_error(detail::buffer<char>& out, int error_code,
								  const char* message) noexcept {
	FMT_TRY {
		auto ec = std::error_code(error_code, std::generic_category());
		detail::write(appender(out), std::system_error(ec, message).what());
		_rt;
	}
	FMT_CATCH(...) {}
	format_error_code(out, error_code, message);
}
FMT_FUNC void report_system_error(int error_code,
								  const char* message) noexcept {
	report_error(format_system_error, error_code, message);
}
FMT_FUNC auto vformat(string_view fmt, format_args args) -> std::string {
	auto buffer = memory_buffer();
	detail::vformat_to(buffer, fmt, args);
	_rt to_string(buffer);
}
namespace detail {
	_tp<_tn T> struct span {
		T* data;
		size_t size;
	};
#ifdef _WIN32
	inline void flockfile(FILE* f) {
		_lock_file(f);
	}
	inline void funlockfile(FILE* f) {
		_unlock_file(f);
	}
	inline int getc_unlocked(FILE* f) {
		_rt _fgetc_nolock(f);
	}
#endif
	_tp<_tn F> class file_base {
	public:
		F* file_;

	public:
		file_base(F* file)
			: file_(file) {}
		operator F*() const { _rt file_; }
		auto get() -> int {
			int result = getc_unlocked(file_);
			if (result == EOF && ferror(file_) != 0)
				FMT_THROW(system_error(errno, FMT_STRING("getc failed")));
			_rt result;
		}
		void unget(char c) {
			if (ungetc(c, file_) == EOF)
				FMT_THROW(system_error(errno, FMT_STRING("ungetc failed")));
		}
		void flush() { fflush(this->file_); }
	};
	_tp<_tn F> class glibc_file : public file_base<F> {
	private:
		enum {
			line_buffered = 0x200,
			unbuffered = 2
		};

	public:
		using file_base<F>::file_base;
		auto is_buffered() const -> bool {
			_rt(this->file_->_flags & unbuffered) == 0;
		}
		void init_buffer() {
			if (this->file_->_IO_write_ptr) _rt;
			putc_unlocked(0, this->file_);
			--this->file_->_IO_write_ptr;
		}
		auto get_read_buffer() const -> span<const char> {
			auto ptr = this->file_->_IO_read_ptr;
			_rt{ ptr, to_unsigned(this->file_->_IO_read_end - ptr) };
		}
		auto get_write_buffer() const -> span<char> {
			auto ptr = this->file_->_IO_write_ptr;
			_rt{ ptr, to_unsigned(this->file_->_IO_buf_end - ptr) };
		}
		void advance_write_buffer(size_t size) { this->file_->_IO_write_ptr += size; }
		bool needs_flush() const {
			if ((this->file_->_flags & line_buffered) == 0) _rt false;
			char* end = this->file_->_IO_write_end;
			_rt memchr(end, '\n', to_unsigned(this->file_->_IO_write_ptr - end));
		}
		void flush() { fflush_unlocked(this->file_); }
	};
	_tp<_tn F> class apple_file : public file_base<F> {
	private:
		enum {
			line_buffered = 1,
			unbuffered = 2
		};

	public:
		using file_base<F>::file_base;
		auto is_buffered() const -> bool {
			_rt(this->file_->_flags & unbuffered) == 0;
		}
		void init_buffer() {
			if (this->file_->_p) _rt;
			putc_unlocked(0, this->file_);
			--this->file_->_p;
			++this->file_->_w;
		}
		auto get_read_buffer() const -> span<const char> {
			_rt{ reinterpret_cast<char*>(this->file_->_p),
				 to_unsigned(this->file_->_r) };
		}
		auto get_write_buffer() const -> span<char> {
			_rt{ reinterpret_cast<char*>(this->file_->_p),
				 to_unsigned(this->file_->_bf._base + this->file_->_bf._size - this->file_->_p) };
		}
		void advance_write_buffer(size_t size) {
			this->file_->_p += size;
			this->file_->_w -= size;
		}
		bool needs_flush() const {
			if ((this->file_->_flags & line_buffered) == 0) _rt false;
			_rt memchr(this->file_->_p + this->file_->_w, '\n',
					   to_unsigned(-this->file_->_w));
		}
	};
	_tp<_tn F> class fallback_file : public file_base<F> {
	private:
		char next_;
		bool has_next_ = false;

	public:
		using file_base<F>::file_base;
		auto is_buffered() const -> bool { _rt false; }
		auto needs_flush() const -> bool { _rt false; }
		void init_buffer() {}
		auto get_read_buffer() const -> span<const char> {
			_rt{ &next_, has_next_ ? 1u : 0u };
		}
		auto get_write_buffer() const -> span<char> { _rt{ nullptr, 0 }; }
		void advance_write_buffer(size_t) {}
		auto get() -> int {
			has_next_ = false;
			_rt file_base<F>::get();
		}
		void unget(char c) {
			file_base<F>::unget(c);
			next_ = c;
			has_next_ = true;
		}
	};
	_tp<_tn F, FMT_ENABLE_IF(sizeof(F::_p) != 0)> auto get_file(F* f, int) -> apple_file<F> {
		_rt f;
	}
	_tp<_tn F, FMT_ENABLE_IF(sizeof(F::_IO_read_ptr) != 0)> inline auto get_file(F* f, int) -> glibc_file<F> {
		_rt f;
	}
	inline auto get_file(FILE* f, ...) -> fallback_file<FILE> {
		_rt f;
	}
	using file_ref = decltype(get_file(_sc<FILE*>(nullptr), 0));
	class file_print_buffer : public buffer<char> {
	private:
		file_ref file_;
		static void grow(buffer<char>& base, size_t) {
			auto& self = _sc<file_print_buffer&>(base);
			self.file_.advance_write_buffer(self.size());
			if (self.file_.get_write_buffer().size == 0) self.file_.flush();
			auto buf = self.file_.get_write_buffer();
			FMT_ASSERT(buf.size > 0, "");
			self.set(buf.data, buf.size);
			self.clear();
		}

	public:
		explicit file_print_buffer(FILE* f)
			: buffer(grow, size_t()), file_(f) {
			flockfile(f);
			file_.init_buffer();
			auto buf = file_.get_write_buffer();
			set(buf.data, buf.size);
		}
		~file_print_buffer() {
			file_.advance_write_buffer(size());
			bool flush = file_.needs_flush();
			funlockfile(file_);
			if (flush) fflush(file_);
		}
	};
#if !defined(_WIN32) || defined(FMT_WINDOWS_NO_WCHAR)
	FMT_FUNC auto write_console(int, string_view) -> bool {
		_rt false;
	}
#else
	using dword = conditional_t<sizeof(long) == 4, unsigned long, unsigned>;
	extern "C" __declspec(dllimport) int __stdcall WriteConsoleW(
		void*, const void*, dword, dword*, void*);
	FMT_FUNC bool write_console(int fd, string_view text) {
		auto u16 = utf8_to_utf16(text);
		_rt WriteConsoleW(reinterpret_cast<void*>(_get_osfhandle(fd)), u16.c_str(),
						  _sc<dword>(u16.size()), nullptr, nullptr)
			!= 0;
	}
#endif
#ifdef _WIN32
	FMT_FUNC void vprint_mojibake(std::FILE* f, string_view fmt, format_args args,
								  bool newline) {
		auto buffer = memory_buffer();
		detail::vformat_to(buffer, fmt, args);
		if (newline) buffer.push_back('\n');
		fwrite_fully(buffer.data(), buffer.size(), f);
	}
#endif
	FMT_FUNC void print(std::FILE* f, string_view text) {
#ifdef _WIN32
		int fd = _fileno(f);
		if (_isatty(fd)) {
			std::fflush(f);
			if (write_console(fd, text)) _rt;
		}
#endif
		fwrite_fully(text.data(), text.size(), f);
	}
} // namespace detail
FMT_FUNC void vprint(std::FILE* f, string_view fmt, format_args args) {
	if (detail::file_ref(f).is_buffered()) {
		auto&& buffer = detail::file_print_buffer(f);
		_rt detail::vformat_to(buffer, fmt, args);
	}
	auto buffer = memory_buffer();
	detail::vformat_to(buffer, fmt, args);
	detail::print(f, { buffer.data(), buffer.size() });
}
FMT_FUNC void vprintln(std::FILE* f, string_view fmt, format_args args) {
	auto buffer = memory_buffer();
	detail::vformat_to(buffer, fmt, args);
	buffer.push_back('\n');
	detail::print(f, { buffer.data(), buffer.size() });
}
FMT_FUNC void vprint(string_view fmt, format_args args) {
	vprint(stdout, fmt, args);
}
namespace detail {
	struct singleton {
		unsigned char upper;
		unsigned char lower_count;
	};
	inline auto is_printable(uint16_t x, const singleton* singletons,
							 size_t singletons_size,
							 const unsigned char* singleton_lowers,
							 const unsigned char* normal, size_t normal_size)
		-> bool {
		auto upper = x >> 8;
		auto lower_start = 0;
		for (size_t i = 0; i < singletons_size; ++i) {
			auto s = singletons[i];
			auto lower_end = lower_start + s.lower_count;
			if (upper < s.upper) break;
			if (upper == s.upper) {
				for (auto j = lower_start; j < lower_end; ++j) {
					if (singleton_lowers[j] == (x & 0xff)) _rt false;
				}
			}
			lower_start = lower_end;
		}
		auto xsigned = _sc<int>(x);
		auto current = true;
		for (size_t i = 0; i < normal_size; ++i) {
			auto v = _sc<int>(normal[i]);
			auto len = (v & 0x80) != 0 ? (v & 0x7f) << 8 | normal[++i] : v;
			xsigned -= len;
			if (xsigned < 0) break;
			current = !current;
		}
		_rt current;
	}
	FMT_FUNC auto is_printable(uint32_t cp) -> bool {
		// clang-format off
		static _cx singleton singletons0[] = {{ 0x00, 1 },{ 0x03, 5 },{ 0x05, 6 },{ 0x06, 3 },{ 0x07, 6 },{ 0x08, 8 },{ 0x09, 17 },{ 0x0a, 28 },{ 0x0b, 25 },{ 0x0c, 20 },{ 0x0d, 16 },{ 0x0e, 13 },{ 0x0f, 4 },{ 0x10, 3 },{ 0x12, 18 },{ 0x13, 9 },{ 0x16, 1 },{ 0x17, 5 },{ 0x18, 2 },{ 0x19, 3 },{ 0x1a, 7 },{ 0x1c, 2 },{ 0x1d, 1 },{ 0x1f, 22 },{ 0x20, 3 },{ 0x2b, 3 },{ 0x2c, 2 },{ 0x2d, 11 },{ 0x2e, 1 },{ 0x30, 3 },{ 0x31, 2 },{ 0x32, 1 },{ 0xa7, 2 },{ 0xa9, 2 },{ 0xaa, 4 },{ 0xab, 8 },{ 0xfa, 2 },{ 0xfb, 5 },{ 0xfd, 4 },{ 0xfe, 3 },{ 0xff, 9 },		};
		static _cx unsigned char singletons0_lower[] = {0xad,0x78,0x79,0x8b,0x8d,0xa2,0x30,0x57,0x58,0x8b,0x8c,0x90,0x1c,0x1d,0xdd,0x0e,0x0f,0x4b,0x4c,0xfb,0xfc,0x2e,0x2f,0x3f,0x5c,0x5d,0x5f,0xb5,0xe2,0x84,0x8d,0x8e,0x91,0x92,0xa9,0xb1,0xba,0xbb,0xc5,0xc6,0xc9,0xca,0xde,0xe4,0xe5,0xff,0x00,0x04,0x11,0x12,0x29,0x31,0x34,0x37,0x3a,0x3b,0x3d,0x49,0x4a,0x5d,0x84,0x8e,0x92,0xa9,0xb1,0xb4,0xba,0xbb,0xc6,0xca,0xce,0xcf,0xe4,0xe5,0x00,0x04,0x0d,0x0e,0x11,0x12,0x29,0x31,0x34,0x3a,0x3b,0x45,0x46,0x49,0x4a,0x5e,0x64,0x65,0x84,0x91,0x9b,0x9d,0xc9,0xce,0xcf,0x0d,0x11,0x29,0x45,0x49,0x57,0x64,0x65,0x8d,0x91,0xa9,0xb4,0xba,0xbb,0xc5,0xc9,0xdf,0xe4,0xe5,0xf0,0x0d,0x11,0x45,0x49,0x64,0x65,0x80,0x84,0xb2,0xbc,0xbe,0xbf,0xd5,0xd7,0xf0,0xf1,0x83,0x85,0x8b,0xa4,0xa6,0xbe,0xbf,0xc5,0xc7,0xce,0xcf,0xda,0xdb,0x48,0x98,0xbd,0xcd,0xc6,0xce,0xcf,0x49,0x4e,0x4f,0x57,0x59,0x5e,0x5f,0x89,0x8e,0x8f,0xb1,0xb6,0xb7,0xbf,0xc1,0xc6,0xc7,0xd7,0x11,0x16,0x17,0x5b,0x5c,0xf6,0xf7,0xfe,0xff,0x80,0x0d,0x6d,0x71,0xde,0xdf,0x0e,0x0f,0x1f,0x6e,0x6f,0x1c,0x1d,0x5f,0x7d,0x7e,0xae,0xaf,0xbb,0xbc,0xfa,0x16,0x17,0x1e,0x1f,0x46,0x47,0x4e,0x4f,0x58,0x5a,0x5c,0x5e,0x7e,0x7f,0xb5,0xc5,0xd4,0xd5,0xdc,0xf0,0xf1,0xf5,0x72,0x73,0x8f,0x74,0x75,0x96,0x2f,0x5f,0x26,0x2e,0x2f,0xa7,0xaf,0xb7,0xbf,0xc7,0xcf,0xd7,0xdf,0x9a,0x40,0x97,0x98,0x30,0x8f,0x1f,0xc0,0xc1,0xce,0xff,0x4e,0x4f,0x5a,0x5b,0x07,0x08,0x0f,0x10,0x27,0x2f,0xee,0xef,0x6e,0x6f,0x37,0x3d,0x3f,0x42,0x45,0x90,0x91,0xfe,0xff,0x53,0x67,0x75,0xc8,0xc9,0xd0,0xd1,0xd8,0xd9,0xe7,0xfe,0xff,		};
		static _cx singleton singletons1[] = {{ 0x00, 6 },{ 0x01, 1 },{ 0x03, 1 },{ 0x04, 2 },{ 0x08, 8 },{ 0x09, 2 },{ 0x0a, 5 },{ 0x0b, 2 },{ 0x0e, 4 },{ 0x10, 1 },{ 0x11, 2 },{ 0x12, 5 },{ 0x13, 17 },{ 0x14, 1 },{ 0x15, 2 },{ 0x17, 2 },{ 0x19, 13 },{ 0x1c, 5 },{ 0x1d, 8 },{ 0x24, 1 },{ 0x6a, 3 },{ 0x6b, 2 },{ 0xbc, 2 },{ 0xd1, 2 },{ 0xd4, 12 },{ 0xd5, 9 },{ 0xd6, 2 },{ 0xd7, 2 },{ 0xda, 1 },{ 0xe0, 5 },{ 0xe1, 2 },{ 0xe8, 2 },{ 0xee, 32 },{ 0xf0, 4 },{ 0xf8, 2 },{ 0xf9, 2 },{ 0xfa, 2 },{ 0xfb, 1 },		};
		static _cx unsigned char singletons1_lower[] = {0x0c,0x27,0x3b,0x3e,0x4e,0x4f,0x8f,0x9e,0x9e,0x9f,0x06,0x07,0x09,0x36,0x3d,0x3e,0x56,0xf3,0xd0,0xd1,0x04,0x14,0x18,0x36,0x37,0x56,0x57,0x7f,0xaa,0xae,0xaf,0xbd,0x35,0xe0,0x12,0x87,0x89,0x8e,0x9e,0x04,0x0d,0x0e,0x11,0x12,0x29,0x31,0x34,0x3a,0x45,0x46,0x49,0x4a,0x4e,0x4f,0x64,0x65,0x5c,0xb6,0xb7,0x1b,0x1c,0x07,0x08,0x0a,0x0b,0x14,0x17,0x36,0x39,0x3a,0xa8,0xa9,0xd8,0xd9,0x09,0x37,0x90,0x91,0xa8,0x07,0x0a,0x3b,0x3e,0x66,0x69,0x8f,0x92,0x6f,0x5f,0xee,0xef,0x5a,0x62,0x9a,0x9b,0x27,0x28,0x55,0x9d,0xa0,0xa1,0xa3,0xa4,0xa7,0xa8,0xad,0xba,0xbc,0xc4,0x06,0x0b,0x0c,0x15,0x1d,0x3a,0x3f,0x45,0x51,0xa6,0xa7,0xcc,0xcd,0xa0,0x07,0x19,0x1a,0x22,0x25,0x3e,0x3f,0xc5,0xc6,0x04,0x20,0x23,0x25,0x26,0x28,0x33,0x38,0x3a,0x48,0x4a,0x4c,0x50,0x53,0x55,0x56,0x58,0x5a,0x5c,0x5e,0x60,0x63,0x65,0x66,0x6b,0x73,0x78,0x7d,0x7f,0x8a,0xa4,0xaa,0xaf,0xb0,0xc0,0xd0,0xae,0xaf,0x79,0xcc,0x6e,0x6f,0x93,		};
		static _cx unsigned char normal0[] = {0x00,0x20,0x5f,0x22,0x82,0xdf,0x04,0x82,0x44,0x08,0x1b,0x04,0x06,0x11,0x81,0xac,0x0e,0x80,0xab,0x35,0x28,0x0b,0x80,0xe0,0x03,0x19,0x08,0x01,0x04,0x2f,0x04,0x34,0x04,0x07,0x03,0x01,0x07,0x06,0x07,0x11,0x0a,0x50,0x0f,0x12,0x07,0x55,0x07,0x03,0x04,0x1c,0x0a,0x09,0x03,0x08,0x03,0x07,0x03,0x02,0x03,0x03,0x03,0x0c,0x04,0x05,0x03,0x0b,0x06,0x01,0x0e,0x15,0x05,0x3a,0x03,0x11,0x07,0x06,0x05,0x10,0x07,0x57,0x07,0x02,0x07,0x15,0x0d,0x50,0x04,0x43,0x03,0x2d,0x03,0x01,0x04,0x11,0x06,0x0f,0x0c,0x3a,0x04,0x1d,0x25,0x5f,0x20,0x6d,0x04,0x6a,0x25,0x80,0xc8,0x05,0x82,0xb0,0x03,0x1a,0x06,0x82,0xfd,0x03,0x59,0x07,0x15,0x0b,0x17,0x09,0x14,0x0c,0x14,0x0c,0x6a,0x06,0x0a,0x06,0x1a,0x06,0x59,0x07,0x2b,0x05,0x46,0x0a,0x2c,0x04,0x0c,0x04,0x01,0x03,0x31,0x0b,0x2c,0x04,0x1a,0x06,0x0b,0x03,0x80,0xac,0x06,0x0a,0x06,0x21,0x3f,0x4c,0x04,0x2d,0x03,0x74,0x08,0x3c,0x03,0x0f,0x03,0x3c,0x07,0x38,0x08,0x2b,0x05,0x82,0xff,0x11,0x18,0x08,0x2f,0x11,0x2d,0x03,0x20,0x10,0x21,0x0f,0x80,0x8c,0x04,0x82,0x97,0x19,0x0b,0x15,0x88,0x94,0x05,0x2f,0x05,0x3b,0x07,0x02,0x0e,0x18,0x09,0x80,0xb3,0x2d,0x74,0x0c,0x80,0xd6,0x1a,0x0c,0x05,0x80,0xff,0x05,0x80,0xdf,0x0c,0xee,0x0d,0x03,0x84,0x8d,0x03,0x37,0x09,0x81,0x5c,0x14,0x80,0xb8,0x08,0x80,0xcb,0x2a,0x38,0x03,0x0a,0x06,0x38,0x08,0x46,0x08,0x0c,0x06,0x74,0x0b,0x1e,0x03,0x5a,0x04,0x59,0x09,0x80,0x83,0x18,0x1c,0x0a,0x16,0x09,0x4c,0x04,0x80,0x8a,0x06,0xab,0xa4,0x0c,0x17,0x04,0x31,0xa1,0x04,0x81,0xda,0x26,0x07,0x0c,0x05,0x05,0x80,0xa5,0x11,0x81,0x6d,0x10,0x78,0x28,0x2a,0x06,0x4c,0x04,0x80,0x8d,0x04,0x80,0xbe,0x03,0x1b,0x03,0x0f,0x0d,		};		static _cx unsigned char normal1[] = {0x5e,0x22,0x7b,0x05,0x03,0x04,0x2d,0x03,0x66,0x03,0x01,0x2f,0x2e,0x80,0x82,0x1d,0x03,0x31,0x0f,0x1c,0x04,0x24,0x09,0x1e,0x05,0x2b,0x05,0x44,0x04,0x0e,0x2a,0x80,0xaa,0x06,0x24,0x04,0x24,0x04,0x28,0x08,0x34,0x0b,0x01,0x80,0x90,0x81,0x37,0x09,0x16,0x0a,0x08,0x80,0x98,0x39,0x03,0x63,0x08,0x09,0x30,0x16,0x05,0x21,0x03,0x1b,0x05,0x01,0x40,0x38,0x04,0x4b,0x05,0x2f,0x04,0x0a,0x07,0x09,0x07,0x40,0x20,0x27,0x04,0x0c,0x09,0x36,0x03,0x3a,0x05,0x1a,0x07,0x04,0x0c,0x07,0x50,0x49,0x37,0x33,0x0d,0x33,0x07,0x2e,0x08,0x0a,0x81,0x26,0x52,0x4e,0x28,0x08,0x2a,0x56,0x1c,0x14,0x17,0x09,0x4e,0x04,0x1e,0x0f,0x43,0x0e,0x19,0x07,0x0a,0x06,0x48,0x08,0x27,0x09,0x75,0x0b,0x3f,0x41,0x2a,0x06,0x3b,0x05,0x0a,0x06,0x51,0x06,0x01,0x05,0x10,0x03,0x05,0x80,0x8b,0x62,0x1e,0x48,0x08,0x0a,0x80,0xa6,0x5e,0x22,0x45,0x0b,0x0a,0x06,0x0d,0x13,0x39,0x07,0x0a,0x36,0x2c,0x04,0x10,0x80,0xc0,0x3c,0x64,0x53,0x0c,0x48,0x09,0x0a,0x46,0x45,0x1b,0x48,0x08,0x53,0x1d,0x39,0x81,0x07,0x46,0x0a,0x1d,0x03,0x47,0x49,0x37,0x03,0x0e,0x08,0x0a,0x06,0x39,0x07,0x0a,0x81,0x36,0x19,0x80,0xb7,0x01,0x0f,0x32,0x0d,0x83,0x9b,0x66,0x75,0x0b,0x80,0xc4,0x8a,0xbc,0x84,0x2f,0x8f,0xd1,0x82,0x47,0xa1,0xb9,0x82,0x39,0x07,0x2a,0x04,0x02,0x60,0x26,0x0a,0x46,0x0a,0x28,0x05,0x13,0x82,0xb0,0x5b,0x65,0x4b,0x04,0x39,0x07,0x11,0x40,0x05,0x0b,0x02,0x0e,0x97,0xf8,0x08,0x84,0xd6,0x2a,0x09,0xa2,0xf7,0x81,0x1f,0x31,0x03,0x11,0x04,0x08,0x81,0x8c,0x89,0x04,0x6b,0x05,0x0d,0x03,0x09,0x07,0x10,0x93,0x60,0x80,0xf6,0x0a,0x73,0x08,0x6e,0x17,0x46,0x80,0x9a,0x14,0x0c,0x57,0x09,0x19,0x80,0x87,0x81,0x47,0x03,0x85,0x42,0x0f,0x15,0x85,0x50,0x2b,0x80,0xd5,0x2d,0x03,0x1a,0x04,0x02,0x81,0x70,0x3a,0x05,0x01,0x85,0x00,0x80,0xd7,0x29,0x4c,0x04,0x0a,0x04,0x02,0x83,0x11,0x44,0x4c,0x3d,0x80,0xc2,0x3c,0x06,0x01,0x04,0x55,0x05,0x1b,0x34,0x02,0x81,0x0e,0x2c,0x04,0x64,0x0c,0x56,0x0a,0x80,0xae,0x38,0x1d,0x0d,0x2c,0x04,0x09,0x07,0x02,0x0e,0x06,0x80,0x9a,0x83,0xd8,0x08,0x0d,0x03,0x0d,0x03,0x74,0x0c,0x59,0x07,0x0c,0x14,0x0c,0x04,0x38,0x08,0x0a,0x06,0x28,0x08,0x22,0x4e,0x81,0x54,0x0c,0x15,0x03,0x03,0x05,0x07,0x09,0x19,0x07,0x07,0x09,0x03,0x0d,0x07,0x29,0x80,0xcb,0x25,0x0a,0x84,0x06,		};
		// clang-format on
		auto lower = _sc<uint16_t>(cp);
		if (cp < 0x10000) {
			_rt is_printable(lower, singletons0,
							 sizeof(singletons0) / sizeof(*singletons0),
							 singletons0_lower, normal0, sizeof(normal0));
		}
		if (cp < 0x20000) {
			_rt is_printable(lower, singletons1,
							 sizeof(singletons1) / sizeof(*singletons1),
							 singletons1_lower, normal1, sizeof(normal1));
		}
		if (0x2a6de <= cp && cp < 0x2a700) _rt false;
		if (0x2b735 <= cp && cp < 0x2b740) _rt false;
		if (0x2b81e <= cp && cp < 0x2b820) _rt false;
		if (0x2cea2 <= cp && cp < 0x2ceb0) _rt false;
		if (0x2ebe1 <= cp && cp < 0x2f800) _rt false;
		if (0x2fa1e <= cp && cp < 0x30000) _rt false;
		if (0x3134b <= cp && cp < 0xe0100) _rt false;
		if (0xe01f0 <= cp && cp < 0x110000) _rt false;
		_rt cp < 0x110000;
	}
} // namespace detail
FMT_END_NAMESPACE
#undef _tn
#endif
