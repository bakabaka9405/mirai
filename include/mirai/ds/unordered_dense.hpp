#ifndef ANKERL_UNORDERED_DENSE_H
#define ANKERL_UNORDERED_DENSE_H
#define ANKERL_UNORDERED_DENSE_VERSION_MAJOR 4
#define ANKERL_UNORDERED_DENSE_VERSION_MINOR 4
#define ANKERL_UNORDERED_DENSE_VERSION_PATCH 0
#define ANKERL_UNORDERED_DENSE_VERSION_CONCAT1(major, minor, patch) v##major##_##minor##_##patch
#define ANKERL_UNORDERED_DENSE_VERSION_CONCAT(major, minor, patch) ANKERL_UNORDERED_DENSE_VERSION_CONCAT1(major, minor, patch)
#define ANKERL_UNORDERED_DENSE_NAMESPACE   \
	ANKERL_UNORDERED_DENSE_VERSION_CONCAT( \
		ANKERL_UNORDERED_DENSE_VERSION_MAJOR, ANKERL_UNORDERED_DENSE_VERSION_MINOR, ANKERL_UNORDERED_DENSE_VERSION_PATCH)
#if defined(_MSVC_LANG)
#define ANKERL_UNORDERED_DENSE_CPP_VERSION _MSVC_LANG
#else
#define ANKERL_UNORDERED_DENSE_CPP_VERSION __cplusplus
#endif
#if defined(__GNUC__)
#define ANKERL_UNORDERED_DENSE_PACK(decl) decl __attribute__((__packed__))
#elif defined(_MSC_VER)
#define ANKERL_UNORDERED_DENSE_PACK(decl) __pragma(pack(push, 1)) decl __pragma(pack(pop))
#endif
#if (defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)) && defined(MR_DEBUG)
#define ANKERL_UNORDERED_DENSE_HAS_EXCEPTIONS() 1
#else
#define ANKERL_UNORDERED_DENSE_HAS_EXCEPTIONS() 0
#endif
#ifdef _MSC_VER
#define ANKERL_UNORDERED_DENSE_NOINLINE __declspec(noinline)
#else
#define ANKERL_UNORDERED_DENSE_NOINLINE __attribute__((noinline))
#endif
#if !defined(ANKERL_UNORDERED_DENSE_EXPORT)
#define ANKERL_UNORDERED_DENSE_EXPORT
#endif
#if ANKERL_UNORDERED_DENSE_CPP_VERSION < 201703L
#error ankerl::unordered_dense requires C++17 or higher
#else
#include <array>
#include <cstdint>
#include <cstring>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>
#if ANKERL_UNORDERED_DENSE_HAS_EXCEPTIONS() == 0
#include <cstdlib>
#endif
#if defined(__has_include)
#if __has_include(<memory_resource>)
#define ANKERL_UNORDERED_DENSE_PMR std::pmr
#include <memory_resource>
#elif __has_include(<experimental/memory_resource>)
#define ANKERL_UNORDERED_DENSE_PMR std::experimental::pmr
#include <experimental/memory_resource>
#endif
#endif
#if defined(_MSC_VER) && defined(_M_X64)
#include <intrin.h>
#pragma intrinsic(_umul128)
#endif
#if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__clang__)
#define ANKERL_UNORDERED_DENSE_LIKELY(x) __builtin_expect(x, 1)
#define ANKERL_UNORDERED_DENSE_UNLIKELY(x) __builtin_expect(x, 0)
#else
#define ANKERL_UNORDERED_DENSE_LIKELY(x) (x)
#define ANKERL_UNORDERED_DENSE_UNLIKELY(x) (x)
#endif
namespace ankerl::unordered_dense {
	inline namespace ANKERL_UNORDERED_DENSE_NAMESPACE {
		namespace detail {
#if ANKERL_UNORDERED_DENSE_HAS_EXCEPTIONS()
			[[noreturn]] inline ANKERL_UNORDERED_DENSE_NOINLINE void on_error_key_not_found() {
				throw std::out_of_range("ankerl::unordered_dense::map::at(): key not found");
			}
			[[noreturn]] inline ANKERL_UNORDERED_DENSE_NOINLINE void on_error_bucket_overflow() {
				throw std::overflow_error("ankerl::unordered_dense: reached max bucket size, cannot increase size");
			}
			[[noreturn]] inline ANKERL_UNORDERED_DENSE_NOINLINE void on_error_too_many_elements() {
				throw std::out_of_range("ankerl::unordered_dense::map::replace(): too many elements");
			}
#else
			[[noreturn]] inline void on_error_key_not_found() {
				abort();
			}
			[[noreturn]] inline void on_error_bucket_overflow() {
				abort();
			}
			[[noreturn]] inline void on_error_too_many_elements() {
				abort();
			}
#endif
		} // namespace detail
		namespace detail::wyhash {
			inline void mum(uint64_t* a, uint64_t* b) {
#if defined(__SIZEOF_INT128__)
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
				if (ANKERL_UNORDERED_DENSE_LIKELY(len <= 16)) {
					if (ANKERL_UNORDERED_DENSE_LIKELY(len >= 4)) {
						a = (r4(p) << 32U) | r4(p + ((len >> 3U) << 2U));
						b = (r4(p + len - 4) << 32U) | r4(p + len - 4 - ((len >> 3U) << 2U));
					}
					else if (ANKERL_UNORDERED_DENSE_LIKELY(len > 0)) {
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
					if (ANKERL_UNORDERED_DENSE_UNLIKELY(i > 48)) {
						uint64_t see1 = seed;
						uint64_t see2 = seed;
						do {
							seed = mix(r8(p) ^ secret[1], r8(p + 8) ^ seed);
							see1 = mix(r8(p + 16) ^ secret[2], r8(p + 24) ^ see1);
							see2 = mix(r8(p + 32) ^ secret[3], r8(p + 40) ^ see2);
							p += 48;
							i -= 48;
						} while (ANKERL_UNORDERED_DENSE_LIKELY(i > 48));
						seed ^= see1 ^ see2;
					}
					while (ANKERL_UNORDERED_DENSE_UNLIKELY(i > 16)) {
						seed = mix(r8(p) ^ secret[1], r8(p + 8) ^ seed);
						i -= 16;
						p += 16;
					}
					a = r8(p + i - 16);
					b = r8(p + i - 8);
				}
				return mix(secret[1] ^ len, mix(a ^ secret[1], b ^ seed));
			}
			[[nodiscard]] inline auto hash(uint64_t x) -> uint64_t {
				return detail::wyhash::mix(x, UINT64_C(0x9E3779B97F4A7C15));
			}
		} // namespace detail::wyhash
		ANKERL_UNORDERED_DENSE_EXPORT template <typename T, typename Enable = void>
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
#define ANKERL_UNORDERED_DENSE_HASH_STATICCAST(T)                    \
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
		ANKERL_UNORDERED_DENSE_HASH_STATICCAST(bool);
		ANKERL_UNORDERED_DENSE_HASH_STATICCAST(char);
		ANKERL_UNORDERED_DENSE_HASH_STATICCAST(signed char);
		ANKERL_UNORDERED_DENSE_HASH_STATICCAST(unsigned char);
#if ANKERL_UNORDERED_DENSE_CPP_VERSION >= 202002L && defined(__cpp_char8_t)
		ANKERL_UNORDERED_DENSE_HASH_STATICCAST(char8_t);
#endif
		ANKERL_UNORDERED_DENSE_HASH_STATICCAST(char16_t);
		ANKERL_UNORDERED_DENSE_HASH_STATICCAST(char32_t);
		ANKERL_UNORDERED_DENSE_HASH_STATICCAST(wchar_t);
		ANKERL_UNORDERED_DENSE_HASH_STATICCAST(short);
		ANKERL_UNORDERED_DENSE_HASH_STATICCAST(unsigned short);
		ANKERL_UNORDERED_DENSE_HASH_STATICCAST(int);
		ANKERL_UNORDERED_DENSE_HASH_STATICCAST(unsigned int);
		ANKERL_UNORDERED_DENSE_HASH_STATICCAST(long);
		ANKERL_UNORDERED_DENSE_HASH_STATICCAST(long long);
		ANKERL_UNORDERED_DENSE_HASH_STATICCAST(unsigned long);
		ANKERL_UNORDERED_DENSE_HASH_STATICCAST(unsigned long long);
#if defined(__GNUC__) && !defined(__clang__)
#pragma GCC diagnostic pop
#endif
		namespace bucket_type {
			struct standard {
				static constexpr uint32_t dist_inc = 1U << 8U;
				static constexpr uint32_t fingerprint_mask = dist_inc - 1;
				uint32_t m_dist_and_fingerprint;
				uint32_t m_value_idx;
			};
			ANKERL_UNORDERED_DENSE_PACK(struct big {
				static constexpr uint32_t dist_inc = 1U << 8U;
				static constexpr uint32_t fingerprint_mask = dist_inc - 1;
				uint32_t m_dist_and_fingerprint;
				size_t m_value_idx;
			});
		} // namespace bucket_type
		namespace detail {
			struct nonesuch {};
			template <class Default, class AlwaysVoid, template <class...> class Op, class... Args>
			struct detector {
				using value_t = std::false_type;
				using type = Default;
			};
			template <class Default, template <class...> class Op, class... Args>
			struct detector<Default, std::void_t<Op<Args...>>, Op, Args...> {
				using value_t = std::true_type;
				using type = Op<Args...>;
			};
			template <template <class...> class Op, class... Args>
			using is_detected = typename detail::detector<detail::nonesuch, void, Op, Args...>::value_t;
			template <template <class...> class Op, class... Args>
			constexpr bool is_detected_v = is_detected<Op, Args...>::value;
			template <typename T>
			using detect_avalanching = typename T::is_avalanching;
			template <typename T>
			using detect_is_transparent = typename T::is_transparent;
			template <typename T>
			using detect_iterator = typename T::iterator;
			template <typename T>
			using detect_reserve = decltype(std::declval<T&>().reserve(size_t{}));
			template <typename Mapped>
			constexpr bool is_map_v = !std::is_void_v<Mapped>;
			template <typename Hash, typename KeyEqual>
			constexpr bool is_transparent_v = is_detected_v<detect_is_transparent, Hash> && is_detected_v<detect_is_transparent, KeyEqual>;
			template <typename From, typename To1, typename To2>
			constexpr bool is_neither_convertible_v = !std::is_convertible_v<From, To1> && !std::is_convertible_v<From, To2>;
			template <typename T>
			constexpr bool has_reserve = is_detected_v<detect_reserve, T>;
			template <class T>
			struct base_table_type_map {
				using mapped_type = T;
			};
			struct base_table_type_set {};
		} // namespace detail
		template <typename T, typename Allocator = std::allocator<T>, size_t MaxSegmentSizeBytes = 4096>
		class segmented_vector {
			template <bool IsConst>
			class iter_t;

		public:
			using allocator_type = Allocator;
			using pointer = typename std::allocator_traits<allocator_type>::pointer;
			using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
			using difference_type = typename std::allocator_traits<allocator_type>::difference_type;
			using value_type = T;
			using size_type = std::size_t;
			using reference = T&;
			using const_reference = T const&;
			using iterator = iter_t<false>;
			using const_iterator = iter_t<true>;

		private:
			using vec_alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<pointer>;
			std::vector<pointer, vec_alloc> m_blocks{};
			size_t m_size{};
			static constexpr auto num_bits_closest(size_t max_val, size_t s) -> size_t {
				auto f = size_t{ 0 };
				while (s << (f + 1) <= max_val) {
					++f;
				}
				return f;
			}
			using self_t = segmented_vector<T, Allocator, MaxSegmentSizeBytes>;
			static constexpr auto num_bits = num_bits_closest(MaxSegmentSizeBytes, sizeof(T));
			static constexpr auto num_elements_in_block = 1U << num_bits;
			static constexpr auto mask = num_elements_in_block - 1U;
			template <bool IsConst>
			class iter_t {
				using ptr_t = typename std::conditional_t<IsConst, segmented_vector::const_pointer const*, segmented_vector::pointer*>;
				ptr_t m_data{};
				size_t m_idx{};
				template <bool B>
				friend class iter_t;

			public:
				using difference_type = segmented_vector::difference_type;
				using value_type = T;
				using reference = typename std::conditional_t<IsConst, value_type const&, value_type&>;
				using pointer = typename std::conditional_t<IsConst, segmented_vector::const_pointer, segmented_vector::pointer>;
				using iterator_category = std::forward_iterator_tag;
				iter_t() noexcept = default;
				template <bool OtherIsConst, typename = typename std::enable_if<IsConst && !OtherIsConst>::type>
				constexpr iter_t(iter_t<OtherIsConst> const& other) noexcept
					: m_data(other.m_data), m_idx(other.m_idx) {}
				constexpr iter_t(ptr_t data, size_t idx) noexcept
					: m_data(data), m_idx(idx) {}
				template <bool OtherIsConst, typename = typename std::enable_if<IsConst && !OtherIsConst>::type>
				constexpr auto operator=(iter_t<OtherIsConst> const& other) noexcept -> iter_t& {
					m_data = other.m_data;
					m_idx = other.m_idx;
					return *this;
				}
				constexpr auto operator++() noexcept -> iter_t& {
					++m_idx;
					return *this;
				}
				constexpr auto operator+(difference_type diff) noexcept -> iter_t {
					return { m_data, static_cast<size_t>(static_cast<difference_type>(m_idx) + diff) };
				}
				template <bool OtherIsConst>
				constexpr auto operator-(iter_t<OtherIsConst> const& other) noexcept -> difference_type {
					return static_cast<difference_type>(m_idx) - static_cast<difference_type>(other.m_idx);
				}
				constexpr auto operator*() const noexcept -> reference {
					return m_data[m_idx >> num_bits][m_idx & mask];
				}
				constexpr auto operator->() const noexcept -> pointer {
					return &m_data[m_idx >> num_bits][m_idx & mask];
				}
				template <bool O>
				constexpr auto operator==(iter_t<O> const& o) const noexcept -> bool {
					return m_idx == o.m_idx;
				}
				template <bool O>
				constexpr auto operator!=(iter_t<O> const& o) const noexcept -> bool {
					return !(*this == o);
				}
			};
			void increase_capacity() {
				auto ba = Allocator(m_blocks.get_allocator());
				pointer block = std::allocator_traits<Allocator>::allocate(ba, num_elements_in_block);
				m_blocks.push_back(block);
			}
			void append_everything_from(segmented_vector&& other) {
				reserve(size() + other.size());
				for (auto&& o : other) {
					emplace_back(std::move(o));
				}
			}
			void append_everything_from(segmented_vector const& other) {
				reserve(size() + other.size());
				for (auto const& o : other) {
					emplace_back(o);
				}
			}
			void dealloc() {
				auto ba = Allocator(m_blocks.get_allocator());
				for (auto ptr : m_blocks) {
					std::allocator_traits<Allocator>::deallocate(ba, ptr, num_elements_in_block);
				}
			}
			[[nodiscard]] static constexpr auto calc_num_blocks_for_capacity(size_t capacity) {
				return (capacity + num_elements_in_block - 1U) / num_elements_in_block;
			}

		public:
			segmented_vector() = default;
			segmented_vector(Allocator alloc)
				: m_blocks(vec_alloc(alloc)) {}
			segmented_vector(segmented_vector&& other, Allocator alloc)
				: segmented_vector(alloc) {
				*this = std::move(other);
			}
			segmented_vector(segmented_vector const& other, Allocator alloc)
				: m_blocks(vec_alloc(alloc)) {
				append_everything_from(other);
			}
			segmented_vector(segmented_vector&& other) noexcept
				: segmented_vector(std::move(other), get_allocator()) {}
			segmented_vector(segmented_vector const& other) {
				append_everything_from(other);
			}
			auto operator=(segmented_vector const& other) -> segmented_vector& {
				if (this == &other) {
					return *this;
				}
				clear();
				append_everything_from(other);
				return *this;
			}
			auto operator=(segmented_vector&& other) noexcept -> segmented_vector& {
				clear();
				dealloc();
				if (other.get_allocator() == get_allocator()) {
					m_blocks = std::move(other.m_blocks);
					m_size = std::exchange(other.m_size, {});
				}
				else {
					m_blocks = std::vector<pointer, vec_alloc>(vec_alloc(other.get_allocator()));
					append_everything_from(std::move(other));
				}
				return *this;
			}
			~segmented_vector() {
				clear();
				dealloc();
			}
			[[nodiscard]] constexpr auto size() const -> size_t {
				return m_size;
			}
			[[nodiscard]] constexpr auto capacity() const -> size_t {
				return m_blocks.size() * num_elements_in_block;
			}
			[[nodiscard]] constexpr auto operator[](size_t i) const noexcept -> T const& {
				return m_blocks[i >> num_bits][i & mask];
			}
			[[nodiscard]] constexpr auto operator[](size_t i) noexcept -> T& {
				return m_blocks[i >> num_bits][i & mask];
			}
			[[nodiscard]] constexpr auto begin() -> iterator {
				return { m_blocks.data(), 0U };
			}
			[[nodiscard]] constexpr auto begin() const -> const_iterator {
				return { m_blocks.data(), 0U };
			}
			[[nodiscard]] constexpr auto cbegin() const -> const_iterator {
				return { m_blocks.data(), 0U };
			}
			[[nodiscard]] constexpr auto end() -> iterator {
				return { m_blocks.data(), m_size };
			}
			[[nodiscard]] constexpr auto end() const -> const_iterator {
				return { m_blocks.data(), m_size };
			}
			[[nodiscard]] constexpr auto cend() const -> const_iterator {
				return { m_blocks.data(), m_size };
			}
			[[nodiscard]] constexpr auto back() -> reference {
				return operator[](m_size - 1);
			}
			[[nodiscard]] constexpr auto back() const -> const_reference {
				return operator[](m_size - 1);
			}
			void pop_back() {
				back().~T();
				--m_size;
			}
			[[nodiscard]] auto empty() const {
				return 0 == m_size;
			}
			void reserve(size_t new_capacity) {
				m_blocks.reserve(calc_num_blocks_for_capacity(new_capacity));
				while (new_capacity > capacity()) {
					increase_capacity();
				}
			}
			[[nodiscard]] auto get_allocator() const -> allocator_type {
				return allocator_type{ m_blocks.get_allocator() };
			}
			template <class... Args>
			auto emplace_back(Args&&... args) -> reference {
				if (m_size == capacity()) {
					increase_capacity();
				}
				auto* ptr = static_cast<void*>(&operator[](m_size));
				auto& ref = *new (ptr) T(std::forward<Args>(args)...);
				++m_size;
				return ref;
			}
			void clear() {
				if constexpr (!std::is_trivially_destructible_v<T>) {
					for (size_t i = 0, s = size(); i < s; ++i) {
						operator[](i).~T();
					}
				}
				m_size = 0;
			}
			void shrink_to_fit() {
				auto ba = Allocator(m_blocks.get_allocator());
				auto num_blocks_required = calc_num_blocks_for_capacity(m_size);
				while (m_blocks.size() > num_blocks_required) {
					std::allocator_traits<Allocator>::deallocate(ba, m_blocks.back(), num_elements_in_block);
					m_blocks.pop_back();
				}
				m_blocks.shrink_to_fit();
			}
		};
		namespace detail {
			template <class Key,
					  class T,
					  class Hash,
					  class KeyEqual,
					  class AllocatorOrContainer,
					  class Bucket,
					  bool IsSegmented>
			class table : public std::conditional_t<is_map_v<T>, base_table_type_map<T>, base_table_type_set> {
				using underlying_value_type = typename std::conditional_t<is_map_v<T>, std::pair<Key, T>, Key>;
				using underlying_container_type = std::conditional_t<IsSegmented,
																	 segmented_vector<underlying_value_type, AllocatorOrContainer>,
																	 std::vector<underlying_value_type, AllocatorOrContainer>>;

			public:
				using value_container_type = std::
					conditional_t<is_detected_v<detect_iterator, AllocatorOrContainer>, AllocatorOrContainer, underlying_container_type>;

			private:
				using bucket_alloc =
					typename std::allocator_traits<typename value_container_type::allocator_type>::template rebind_alloc<Bucket>;
				using bucket_alloc_traits = std::allocator_traits<bucket_alloc>;
				static constexpr uint8_t initial_shifts = 64 - 2;
				static constexpr float default_max_load_factor = 0.8F;

			public:
				using key_type = Key;
				using value_type = typename value_container_type::value_type;
				using size_type = typename value_container_type::size_type;
				using difference_type = typename value_container_type::difference_type;
				using hasher = Hash;
				using key_equal = KeyEqual;
				using allocator_type = typename value_container_type::allocator_type;
				using reference = typename value_container_type::reference;
				using const_reference = typename value_container_type::const_reference;
				using pointer = typename value_container_type::pointer;
				using const_pointer = typename value_container_type::const_pointer;
				using const_iterator = typename value_container_type::const_iterator;
				using iterator = std::conditional_t<is_map_v<T>, typename value_container_type::iterator, const_iterator>;
				using bucket_type = Bucket;

			private:
				using value_idx_type = decltype(Bucket::m_value_idx);
				using dist_and_fingerprint_type = decltype(Bucket::m_dist_and_fingerprint);
				static_assert(std::is_trivially_destructible_v<Bucket>, "assert there's no need to call destructor / std::destroy");
				static_assert(std::is_trivially_copyable_v<Bucket>, "assert we can just memset / memcpy");
				value_container_type m_values{};
				using bucket_pointer = typename std::allocator_traits<bucket_alloc>::pointer;
				bucket_pointer m_buckets{};
				size_t m_num_buckets = 0;
				size_t m_max_bucket_capacity = 0;
				float m_max_load_factor = default_max_load_factor;
				Hash m_hash{};
				KeyEqual m_equal{};
				uint8_t m_shifts = initial_shifts;
				[[nodiscard]] auto next(value_idx_type bucket_idx) const -> value_idx_type {
					return ANKERL_UNORDERED_DENSE_UNLIKELY(bucket_idx + 1U == m_num_buckets)
							   ? 0
							   : static_cast<value_idx_type>(bucket_idx + 1U);
				}
				[[nodiscard]] static constexpr auto at(bucket_pointer bucket_ptr, size_t offset) -> Bucket& {
					return *(bucket_ptr + static_cast<typename std::allocator_traits<bucket_alloc>::difference_type>(offset));
				}
				[[nodiscard]] static constexpr auto dist_inc(dist_and_fingerprint_type x) -> dist_and_fingerprint_type {
					return static_cast<dist_and_fingerprint_type>(x + Bucket::dist_inc);
				}
				[[nodiscard]] static constexpr auto dist_dec(dist_and_fingerprint_type x) -> dist_and_fingerprint_type {
					return static_cast<dist_and_fingerprint_type>(x - Bucket::dist_inc);
				}
				template <typename K>
				[[nodiscard]] constexpr auto mixed_hash(K const& key) const -> uint64_t {
					if constexpr (is_detected_v<detect_avalanching, Hash>) {
						if constexpr (sizeof(decltype(m_hash(key))) < sizeof(uint64_t)) {
							return m_hash(key) * UINT64_C(0x9ddfea08eb382d69);
						}
						else {
							return m_hash(key);
						}
					}
					else {
						return wyhash::hash(m_hash(key));
					}
				}
				[[nodiscard]] constexpr auto dist_and_fingerprint_from_hash(uint64_t hash) const -> dist_and_fingerprint_type {
					return Bucket::dist_inc | (static_cast<dist_and_fingerprint_type>(hash) & Bucket::fingerprint_mask);
				}
				[[nodiscard]] constexpr auto bucket_idx_from_hash(uint64_t hash) const -> value_idx_type {
					return static_cast<value_idx_type>(hash >> m_shifts);
				}
				[[nodiscard]] static constexpr auto get_key(value_type const& vt) -> key_type const& {
					if constexpr (is_map_v<T>) {
						return vt.first;
					}
					else {
						return vt;
					}
				}
				template <typename K>
				[[nodiscard]] auto next_while_less(K const& key) const -> Bucket {
					auto hash = mixed_hash(key);
					auto dist_and_fingerprint = dist_and_fingerprint_from_hash(hash);
					auto bucket_idx = bucket_idx_from_hash(hash);
					while (dist_and_fingerprint < at(m_buckets, bucket_idx).m_dist_and_fingerprint) {
						dist_and_fingerprint = dist_inc(dist_and_fingerprint);
						bucket_idx = next(bucket_idx);
					}
					return { dist_and_fingerprint, bucket_idx };
				}
				void place_and_shift_up(Bucket bucket, value_idx_type place) {
					while (0 != at(m_buckets, place).m_dist_and_fingerprint) {
						bucket = std::exchange(at(m_buckets, place), bucket);
						bucket.m_dist_and_fingerprint = dist_inc(bucket.m_dist_and_fingerprint);
						place = next(place);
					}
					at(m_buckets, place) = bucket;
				}
				[[nodiscard]] static constexpr auto calc_num_buckets(uint8_t shifts) -> size_t {
					return (std::min)(max_bucket_count(), size_t{ 1 } << (64U - shifts));
				}
				[[nodiscard]] constexpr auto calc_shifts_for_size(size_t s) const -> uint8_t {
					auto shifts = initial_shifts;
					while (shifts > 0 && static_cast<size_t>(static_cast<float>(calc_num_buckets(shifts)) * max_load_factor()) < s) {
						--shifts;
					}
					return shifts;
				}
				void copy_buckets(table const& other) {
					if (empty()) {
						allocate_buckets_from_shift();
						clear_buckets();
					}
					else {
						m_shifts = other.m_shifts;
						allocate_buckets_from_shift();
						std::memcpy(m_buckets, other.m_buckets, sizeof(Bucket) * bucket_count());
					}
				}
				[[nodiscard]] auto is_full() const -> bool {
					return size() > m_max_bucket_capacity;
				}
				void deallocate_buckets() {
					auto ba = bucket_alloc(m_values.get_allocator());
					if (nullptr != m_buckets) {
						bucket_alloc_traits::deallocate(ba, m_buckets, bucket_count());
						m_buckets = nullptr;
					}
					m_num_buckets = 0;
					m_max_bucket_capacity = 0;
				}
				void allocate_buckets_from_shift() {
					auto ba = bucket_alloc(m_values.get_allocator());
					m_num_buckets = calc_num_buckets(m_shifts);
					m_buckets = bucket_alloc_traits::allocate(ba, m_num_buckets);
					if (m_num_buckets == max_bucket_count()) {
						m_max_bucket_capacity = max_bucket_count();
					}
					else {
						m_max_bucket_capacity = static_cast<value_idx_type>(static_cast<float>(m_num_buckets) * max_load_factor());
					}
				}
				void clear_buckets() {
					if (m_buckets != nullptr) {
						std::memset(&*m_buckets, 0, sizeof(Bucket) * bucket_count());
					}
				}
				void clear_and_fill_buckets_from_values() {
					clear_buckets();
					for (value_idx_type value_idx = 0, end_idx = static_cast<value_idx_type>(m_values.size()); value_idx < end_idx;
						 ++value_idx) {
						auto const& key = get_key(m_values[value_idx]);
						auto [dist_and_fingerprint, bucket] = next_while_less(key);
						place_and_shift_up({ dist_and_fingerprint, value_idx }, bucket);
					}
				}
				void increase_size() {
					if (m_max_bucket_capacity == max_bucket_count()) {
						m_values.pop_back();
						on_error_bucket_overflow();
					}
					--m_shifts;
					deallocate_buckets();
					allocate_buckets_from_shift();
					clear_and_fill_buckets_from_values();
				}
				template <typename Op>
				void do_erase(value_idx_type bucket_idx, Op handle_erased_value) {
					auto const value_idx_to_remove = at(m_buckets, bucket_idx).m_value_idx;
					auto next_bucket_idx = next(bucket_idx);
					while (at(m_buckets, next_bucket_idx).m_dist_and_fingerprint >= Bucket::dist_inc * 2) {
						at(m_buckets, bucket_idx) = { dist_dec(at(m_buckets, next_bucket_idx).m_dist_and_fingerprint),
													  at(m_buckets, next_bucket_idx).m_value_idx };
						bucket_idx = std::exchange(next_bucket_idx, next(next_bucket_idx));
					}
					at(m_buckets, bucket_idx) = {};
					handle_erased_value(std::move(m_values[value_idx_to_remove]));
					if (value_idx_to_remove != m_values.size() - 1) {
						auto& val = m_values[value_idx_to_remove];
						val = std::move(m_values.back());
						auto mh = mixed_hash(get_key(val));
						bucket_idx = bucket_idx_from_hash(mh);
						auto const values_idx_back = static_cast<value_idx_type>(m_values.size() - 1);
						while (values_idx_back != at(m_buckets, bucket_idx).m_value_idx) {
							bucket_idx = next(bucket_idx);
						}
						at(m_buckets, bucket_idx).m_value_idx = value_idx_to_remove;
					}
					m_values.pop_back();
				}
				template <typename K, typename Op>
				auto do_erase_key(K&& key, Op handle_erased_value) -> size_t {
					if (empty()) {
						return 0;
					}
					auto [dist_and_fingerprint, bucket_idx] = next_while_less(key);
					while (dist_and_fingerprint == at(m_buckets, bucket_idx).m_dist_and_fingerprint && !m_equal(key, get_key(m_values[at(m_buckets, bucket_idx).m_value_idx]))) {
						dist_and_fingerprint = dist_inc(dist_and_fingerprint);
						bucket_idx = next(bucket_idx);
					}
					if (dist_and_fingerprint != at(m_buckets, bucket_idx).m_dist_and_fingerprint) {
						return 0;
					}
					do_erase(bucket_idx, handle_erased_value);
					return 1;
				}
				template <class K, class M>
				auto do_insert_or_assign(K&& key, M&& mapped) -> std::pair<iterator, bool> {
					auto it_isinserted = try_emplace(std::forward<K>(key), std::forward<M>(mapped));
					if (!it_isinserted.second) {
						it_isinserted.first->second = std::forward<M>(mapped);
					}
					return it_isinserted;
				}
				template <typename... Args>
				auto do_place_element(dist_and_fingerprint_type dist_and_fingerprint, value_idx_type bucket_idx, Args&&... args)
					-> std::pair<iterator, bool> {
					m_values.emplace_back(std::forward<Args>(args)...);
					auto value_idx = static_cast<value_idx_type>(m_values.size() - 1);
					if (ANKERL_UNORDERED_DENSE_UNLIKELY(is_full())) {
						increase_size();
					}
					else {
						place_and_shift_up({ dist_and_fingerprint, value_idx }, bucket_idx);
					}
					return { begin() + static_cast<difference_type>(value_idx), true };
				}
				template <typename K, typename... Args>
				auto do_try_emplace(K&& key, Args&&... args) -> std::pair<iterator, bool> {
					auto hash = mixed_hash(key);
					auto dist_and_fingerprint = dist_and_fingerprint_from_hash(hash);
					auto bucket_idx = bucket_idx_from_hash(hash);
					while (true) {
						auto* bucket = &at(m_buckets, bucket_idx);
						if (dist_and_fingerprint == bucket->m_dist_and_fingerprint) {
							if (m_equal(key, get_key(m_values[bucket->m_value_idx]))) {
								return { begin() + static_cast<difference_type>(bucket->m_value_idx), false };
							}
						}
						else if (dist_and_fingerprint > bucket->m_dist_and_fingerprint) {
							return do_place_element(dist_and_fingerprint,
													bucket_idx,
													std::piecewise_construct,
													std::forward_as_tuple(std::forward<K>(key)),
													std::forward_as_tuple(std::forward<Args>(args)...));
						}
						dist_and_fingerprint = dist_inc(dist_and_fingerprint);
						bucket_idx = next(bucket_idx);
					}
				}
				template <typename K>
				auto do_find(K const& key) -> iterator {
					if (ANKERL_UNORDERED_DENSE_UNLIKELY(empty())) {
						return end();
					}
					auto mh = mixed_hash(key);
					auto dist_and_fingerprint = dist_and_fingerprint_from_hash(mh);
					auto bucket_idx = bucket_idx_from_hash(mh);
					auto* bucket = &at(m_buckets, bucket_idx);
					if (dist_and_fingerprint == bucket->m_dist_and_fingerprint && m_equal(key, get_key(m_values[bucket->m_value_idx]))) {
						return begin() + static_cast<difference_type>(bucket->m_value_idx);
					}
					dist_and_fingerprint = dist_inc(dist_and_fingerprint);
					bucket_idx = next(bucket_idx);
					bucket = &at(m_buckets, bucket_idx);
					if (dist_and_fingerprint == bucket->m_dist_and_fingerprint && m_equal(key, get_key(m_values[bucket->m_value_idx]))) {
						return begin() + static_cast<difference_type>(bucket->m_value_idx);
					}
					dist_and_fingerprint = dist_inc(dist_and_fingerprint);
					bucket_idx = next(bucket_idx);
					bucket = &at(m_buckets, bucket_idx);
					while (true) {
						if (dist_and_fingerprint == bucket->m_dist_and_fingerprint) {
							if (m_equal(key, get_key(m_values[bucket->m_value_idx]))) {
								return begin() + static_cast<difference_type>(bucket->m_value_idx);
							}
						}
						else if (dist_and_fingerprint > bucket->m_dist_and_fingerprint) {
							return end();
						}
						dist_and_fingerprint = dist_inc(dist_and_fingerprint);
						bucket_idx = next(bucket_idx);
						bucket = &at(m_buckets, bucket_idx);
					}
				}
				template <typename K>
				auto do_find(K const& key) const -> const_iterator {
					return const_cast<table*>(this)->do_find(key);
				}
				template <typename K, typename Q = T, std::enable_if_t<is_map_v<Q>, bool> = true>
				auto do_at(K const& key) -> Q& {
					if (auto it = find(key); ANKERL_UNORDERED_DENSE_LIKELY(end() != it)) {
						return it->second;
					}
					on_error_key_not_found();
				}
				template <typename K, typename Q = T, std::enable_if_t<is_map_v<Q>, bool> = true>
				auto do_at(K const& key) const -> Q const& {
					return const_cast<table*>(this)->at(key);
				}

			public:
				explicit table(size_t bucket_count,
							   Hash const& hash = Hash(),
							   KeyEqual const& equal = KeyEqual(),
							   allocator_type const& alloc_or_container = allocator_type())
					: m_values(alloc_or_container), m_hash(hash), m_equal(equal) {
					if (0 != bucket_count) {
						reserve(bucket_count);
					}
					else {
						allocate_buckets_from_shift();
						clear_buckets();
					}
				}
				table()
					: table(0) {}
				table(size_t bucket_count, allocator_type const& alloc)
					: table(bucket_count, Hash(), KeyEqual(), alloc) {}
				table(size_t bucket_count, Hash const& hash, allocator_type const& alloc)
					: table(bucket_count, hash, KeyEqual(), alloc) {}
				explicit table(allocator_type const& alloc)
					: table(0, Hash(), KeyEqual(), alloc) {}
				template <class InputIt>
				table(InputIt first,
					  InputIt last,
					  size_type bucket_count = 0,
					  Hash const& hash = Hash(),
					  KeyEqual const& equal = KeyEqual(),
					  allocator_type const& alloc = allocator_type())
					: table(bucket_count, hash, equal, alloc) {
					insert(first, last);
				}
				template <class InputIt>
				table(InputIt first, InputIt last, size_type bucket_count, allocator_type const& alloc)
					: table(first, last, bucket_count, Hash(), KeyEqual(), alloc) {}
				template <class InputIt>
				table(InputIt first, InputIt last, size_type bucket_count, Hash const& hash, allocator_type const& alloc)
					: table(first, last, bucket_count, hash, KeyEqual(), alloc) {}
				table(table const& other)
					: table(other, other.m_values.get_allocator()) {}
				table(table const& other, allocator_type const& alloc)
					: m_values(other.m_values, alloc), m_max_load_factor(other.m_max_load_factor), m_hash(other.m_hash), m_equal(other.m_equal) {
					copy_buckets(other);
				}
				table(table&& other) noexcept
					: table(std::move(other), other.m_values.get_allocator()) {}
				table(table&& other, allocator_type const& alloc) noexcept
					: m_values(alloc) {
					*this = std::move(other);
				}
				table(std::initializer_list<value_type> ilist,
					  size_t bucket_count = 0,
					  Hash const& hash = Hash(),
					  KeyEqual const& equal = KeyEqual(),
					  allocator_type const& alloc = allocator_type())
					: table(bucket_count, hash, equal, alloc) {
					insert(ilist);
				}
				table(std::initializer_list<value_type> ilist, size_type bucket_count, allocator_type const& alloc)
					: table(ilist, bucket_count, Hash(), KeyEqual(), alloc) {}
				table(std::initializer_list<value_type> init, size_type bucket_count, Hash const& hash, allocator_type const& alloc)
					: table(init, bucket_count, hash, KeyEqual(), alloc) {}
				~table() {
					if (nullptr != m_buckets) {
						auto ba = bucket_alloc(m_values.get_allocator());
						bucket_alloc_traits::deallocate(ba, m_buckets, bucket_count());
					}
				}
				auto operator=(table const& other) -> table& {
					if (&other != this) {
						deallocate_buckets();
						m_values = other.m_values;
						m_max_load_factor = other.m_max_load_factor;
						m_hash = other.m_hash;
						m_equal = other.m_equal;
						m_shifts = initial_shifts;
						copy_buckets(other);
					}
					return *this;
				}
				auto operator=(table&& other) noexcept(noexcept(std::is_nothrow_move_assignable_v<value_container_type> && std::is_nothrow_move_assignable_v<Hash> && std::is_nothrow_move_assignable_v<KeyEqual>)) -> table& {
					if (&other != this) {
						deallocate_buckets();
						m_values = std::move(other.m_values);
						other.m_values.clear();
						if (get_allocator() == other.get_allocator()) {
							m_buckets = std::exchange(other.m_buckets, nullptr);
							m_num_buckets = std::exchange(other.m_num_buckets, 0);
							m_max_bucket_capacity = std::exchange(other.m_max_bucket_capacity, 0);
							m_shifts = std::exchange(other.m_shifts, initial_shifts);
							m_max_load_factor = std::exchange(other.m_max_load_factor, default_max_load_factor);
							m_hash = std::exchange(other.m_hash, {});
							m_equal = std::exchange(other.m_equal, {});
							other.allocate_buckets_from_shift();
							other.clear_buckets();
						}
						else {
							m_max_load_factor = other.m_max_load_factor;
							copy_buckets(other);
							other.clear_buckets();
							m_hash = other.m_hash;
							m_equal = other.m_equal;
						}
					}
					return *this;
				}
				auto operator=(std::initializer_list<value_type> ilist) -> table& {
					clear();
					insert(ilist);
					return *this;
				}
				auto get_allocator() const noexcept -> allocator_type {
					return m_values.get_allocator();
				}
				auto begin() noexcept -> iterator {
					return m_values.begin();
				}
				auto begin() const noexcept -> const_iterator {
					return m_values.begin();
				}
				auto cbegin() const noexcept -> const_iterator {
					return m_values.cbegin();
				}
				auto end() noexcept -> iterator {
					return m_values.end();
				}
				auto cend() const noexcept -> const_iterator {
					return m_values.cend();
				}
				auto end() const noexcept -> const_iterator {
					return m_values.end();
				}
				[[nodiscard]] auto empty() const noexcept -> bool {
					return m_values.empty();
				}
				[[nodiscard]] auto size() const noexcept -> size_t {
					return m_values.size();
				}
				[[nodiscard]] static constexpr auto max_size() noexcept -> size_t {
					if constexpr ((std::numeric_limits<value_idx_type>::max)() == (std::numeric_limits<size_t>::max)()) {
						return size_t{ 1 } << (sizeof(value_idx_type) * 8 - 1);
					}
					else {
						return size_t{ 1 } << (sizeof(value_idx_type) * 8);
					}
				}
				void clear() {
					m_values.clear();
					clear_buckets();
				}
				auto insert(value_type const& value) -> std::pair<iterator, bool> {
					return emplace(value);
				}
				auto insert(value_type&& value) -> std::pair<iterator, bool> {
					return emplace(std::move(value));
				}
				template <class P, std::enable_if_t<std::is_constructible_v<value_type, P&&>, bool> = true>
				auto insert(P&& value) -> std::pair<iterator, bool> {
					return emplace(std::forward<P>(value));
				}
				auto insert(const_iterator, value_type const& value) -> iterator {
					return insert(value).first;
				}
				auto insert(const_iterator, value_type&& value) -> iterator {
					return insert(std::move(value)).first;
				}
				template <class P, std::enable_if_t<std::is_constructible_v<value_type, P&&>, bool> = true>
				auto insert(const_iterator, P&& value) -> iterator {
					return insert(std::forward<P>(value)).first;
				}
				template <class InputIt>
				void insert(InputIt first, InputIt last) {
					while (first != last) {
						insert(*first);
						++first;
					}
				}
				void insert(std::initializer_list<value_type> ilist) {
					insert(ilist.begin(), ilist.end());
				}
				auto extract() && -> value_container_type {
					return std::move(m_values);
				}
				auto replace(value_container_type&& container) {
					if (ANKERL_UNORDERED_DENSE_UNLIKELY(container.size() > max_size())) {
						on_error_too_many_elements();
					}
					auto shifts = calc_shifts_for_size(container.size());
					if (0 == m_num_buckets || shifts < m_shifts || container.get_allocator() != m_values.get_allocator()) {
						m_shifts = shifts;
						deallocate_buckets();
						allocate_buckets_from_shift();
					}
					clear_buckets();
					m_values = std::move(container);
					auto value_idx = value_idx_type{};
					while (value_idx != static_cast<value_idx_type>(m_values.size())) {
						auto const& key = get_key(m_values[value_idx]);
						auto hash = mixed_hash(key);
						auto dist_and_fingerprint = dist_and_fingerprint_from_hash(hash);
						auto bucket_idx = bucket_idx_from_hash(hash);
						bool key_found = false;
						while (true) {
							auto const& bucket = at(m_buckets, bucket_idx);
							if (dist_and_fingerprint > bucket.m_dist_and_fingerprint) {
								break;
							}
							if (dist_and_fingerprint == bucket.m_dist_and_fingerprint && m_equal(key, get_key(m_values[bucket.m_value_idx]))) {
								key_found = true;
								break;
							}
							dist_and_fingerprint = dist_inc(dist_and_fingerprint);
							bucket_idx = next(bucket_idx);
						}
						if (key_found) {
							if (value_idx != static_cast<value_idx_type>(m_values.size() - 1)) {
								m_values[value_idx] = std::move(m_values.back());
							}
							m_values.pop_back();
						}
						else {
							place_and_shift_up({ dist_and_fingerprint, value_idx }, bucket_idx);
							++value_idx;
						}
					}
				}
				template <class M, typename Q = T, std::enable_if_t<is_map_v<Q>, bool> = true>
				auto insert_or_assign(Key const& key, M&& mapped) -> std::pair<iterator, bool> {
					return do_insert_or_assign(key, std::forward<M>(mapped));
				}
				template <class M, typename Q = T, std::enable_if_t<is_map_v<Q>, bool> = true>
				auto insert_or_assign(Key&& key, M&& mapped) -> std::pair<iterator, bool> {
					return do_insert_or_assign(std::move(key), std::forward<M>(mapped));
				}
				template <typename K,
						  typename M,
						  typename Q = T,
						  typename H = Hash,
						  typename KE = KeyEqual,
						  std::enable_if_t<is_map_v<Q> && is_transparent_v<H, KE>, bool> = true>
				auto insert_or_assign(K&& key, M&& mapped) -> std::pair<iterator, bool> {
					return do_insert_or_assign(std::forward<K>(key), std::forward<M>(mapped));
				}
				template <class M, typename Q = T, std::enable_if_t<is_map_v<Q>, bool> = true>
				auto insert_or_assign(const_iterator, Key const& key, M&& mapped) -> iterator {
					return do_insert_or_assign(key, std::forward<M>(mapped)).first;
				}
				template <class M, typename Q = T, std::enable_if_t<is_map_v<Q>, bool> = true>
				auto insert_or_assign(const_iterator, Key&& key, M&& mapped) -> iterator {
					return do_insert_or_assign(std::move(key), std::forward<M>(mapped)).first;
				}
				template <typename K,
						  typename M,
						  typename Q = T,
						  typename H = Hash,
						  typename KE = KeyEqual,
						  std::enable_if_t<is_map_v<Q> && is_transparent_v<H, KE>, bool> = true>
				auto insert_or_assign(const_iterator, K&& key, M&& mapped) -> iterator {
					return do_insert_or_assign(std::forward<K>(key), std::forward<M>(mapped)).first;
				}
				template <class K,
						  typename Q = T,
						  typename H = Hash,
						  typename KE = KeyEqual,
						  std::enable_if_t<!is_map_v<Q> && is_transparent_v<H, KE>, bool> = true>
				auto emplace(K&& key) -> std::pair<iterator, bool> {
					auto hash = mixed_hash(key);
					auto dist_and_fingerprint = dist_and_fingerprint_from_hash(hash);
					auto bucket_idx = bucket_idx_from_hash(hash);
					while (dist_and_fingerprint <= at(m_buckets, bucket_idx).m_dist_and_fingerprint) {
						if (dist_and_fingerprint == at(m_buckets, bucket_idx).m_dist_and_fingerprint && m_equal(key, m_values[at(m_buckets, bucket_idx).m_value_idx])) {
							return { begin() + static_cast<difference_type>(at(m_buckets, bucket_idx).m_value_idx), false };
						}
						dist_and_fingerprint = dist_inc(dist_and_fingerprint);
						bucket_idx = next(bucket_idx);
					}
					return do_place_element(dist_and_fingerprint, bucket_idx, std::forward<K>(key));
				}
				template <class... Args>
				auto emplace(Args&&... args) -> std::pair<iterator, bool> {
					auto& key = get_key(m_values.emplace_back(std::forward<Args>(args)...));
					auto hash = mixed_hash(key);
					auto dist_and_fingerprint = dist_and_fingerprint_from_hash(hash);
					auto bucket_idx = bucket_idx_from_hash(hash);
					while (dist_and_fingerprint <= at(m_buckets, bucket_idx).m_dist_and_fingerprint) {
						if (dist_and_fingerprint == at(m_buckets, bucket_idx).m_dist_and_fingerprint && m_equal(key, get_key(m_values[at(m_buckets, bucket_idx).m_value_idx]))) {
							m_values.pop_back();
							return { begin() + static_cast<difference_type>(at(m_buckets, bucket_idx).m_value_idx), false };
						}
						dist_and_fingerprint = dist_inc(dist_and_fingerprint);
						bucket_idx = next(bucket_idx);
					}
					auto value_idx = static_cast<value_idx_type>(m_values.size() - 1);
					if (ANKERL_UNORDERED_DENSE_UNLIKELY(is_full())) {
						increase_size();
					}
					else {
						place_and_shift_up({ dist_and_fingerprint, value_idx }, bucket_idx);
					}
					return { begin() + static_cast<difference_type>(value_idx), true };
				}
				template <class... Args>
				auto emplace_hint(const_iterator, Args&&... args) -> iterator {
					return emplace(std::forward<Args>(args)...).first;
				}
				template <class... Args, typename Q = T, std::enable_if_t<is_map_v<Q>, bool> = true>
				auto try_emplace(Key const& key, Args&&... args) -> std::pair<iterator, bool> {
					return do_try_emplace(key, std::forward<Args>(args)...);
				}
				template <class... Args, typename Q = T, std::enable_if_t<is_map_v<Q>, bool> = true>
				auto try_emplace(Key&& key, Args&&... args) -> std::pair<iterator, bool> {
					return do_try_emplace(std::move(key), std::forward<Args>(args)...);
				}
				template <class... Args, typename Q = T, std::enable_if_t<is_map_v<Q>, bool> = true>
				auto try_emplace(const_iterator, Key const& key, Args&&... args) -> iterator {
					return do_try_emplace(key, std::forward<Args>(args)...).first;
				}
				template <class... Args, typename Q = T, std::enable_if_t<is_map_v<Q>, bool> = true>
				auto try_emplace(const_iterator, Key&& key, Args&&... args) -> iterator {
					return do_try_emplace(std::move(key), std::forward<Args>(args)...).first;
				}
				template <
					typename K,
					typename... Args,
					typename Q = T,
					typename H = Hash,
					typename KE = KeyEqual,
					std::enable_if_t<is_map_v<Q> && is_transparent_v<H, KE> && is_neither_convertible_v<K&&, iterator, const_iterator>,
									 bool> = true>
				auto try_emplace(K&& key, Args&&... args) -> std::pair<iterator, bool> {
					return do_try_emplace(std::forward<K>(key), std::forward<Args>(args)...);
				}
				template <
					typename K,
					typename... Args,
					typename Q = T,
					typename H = Hash,
					typename KE = KeyEqual,
					std::enable_if_t<is_map_v<Q> && is_transparent_v<H, KE> && is_neither_convertible_v<K&&, iterator, const_iterator>,
									 bool> = true>
				auto try_emplace(const_iterator, K&& key, Args&&... args) -> iterator {
					return do_try_emplace(std::forward<K>(key), std::forward<Args>(args)...).first;
				}
				auto erase(iterator it) -> iterator {
					auto hash = mixed_hash(get_key(*it));
					auto bucket_idx = bucket_idx_from_hash(hash);
					auto const value_idx_to_remove = static_cast<value_idx_type>(it - cbegin());
					while (at(m_buckets, bucket_idx).m_value_idx != value_idx_to_remove) {
						bucket_idx = next(bucket_idx);
					}
					do_erase(bucket_idx, [](value_type&&) {
					});
					return begin() + static_cast<difference_type>(value_idx_to_remove);
				}
				auto extract(iterator it) -> value_type {
					auto hash = mixed_hash(get_key(*it));
					auto bucket_idx = bucket_idx_from_hash(hash);
					auto const value_idx_to_remove = static_cast<value_idx_type>(it - cbegin());
					while (at(m_buckets, bucket_idx).m_value_idx != value_idx_to_remove) {
						bucket_idx = next(bucket_idx);
					}
					auto tmp = std::optional<value_type>{};
					do_erase(bucket_idx, [&tmp](value_type&& val) {
						tmp = std::move(val);
					});
					return std::move(tmp).value();
				}
				template <typename Q = T, std::enable_if_t<is_map_v<Q>, bool> = true>
				auto erase(const_iterator it) -> iterator {
					return erase(begin() + (it - cbegin()));
				}
				template <typename Q = T, std::enable_if_t<is_map_v<Q>, bool> = true>
				auto extract(const_iterator it) -> value_type {
					return extract(begin() + (it - cbegin()));
				}
				auto erase(const_iterator first, const_iterator last) -> iterator {
					auto const idx_first = first - cbegin();
					auto const idx_last = last - cbegin();
					auto const first_to_last = std::distance(first, last);
					auto const last_to_end = std::distance(last, cend());
					auto const mid = idx_first + (std::min)(first_to_last, last_to_end);
					auto idx = idx_first;
					while (idx != mid) {
						erase(begin() + idx);
						++idx;
					}
					idx = idx_last;
					while (idx != mid) {
						--idx;
						erase(begin() + idx);
					}
					return begin() + idx_first;
				}
				auto erase(Key const& key) -> size_t {
					return do_erase_key(key, [](value_type&&) {
					});
				}
				auto extract(Key const& key) -> std::optional<value_type> {
					auto tmp = std::optional<value_type>{};
					do_erase_key(key, [&tmp](value_type&& val) {
						tmp = std::move(val);
					});
					return tmp;
				}
				template <class K, class H = Hash, class KE = KeyEqual, std::enable_if_t<is_transparent_v<H, KE>, bool> = true>
				auto erase(K&& key) -> size_t {
					return do_erase_key(std::forward<K>(key), [](value_type&&) {
					});
				}
				template <class K, class H = Hash, class KE = KeyEqual, std::enable_if_t<is_transparent_v<H, KE>, bool> = true>
				auto extract(K&& key) -> std::optional<value_type> {
					auto tmp = std::optional<value_type>{};
					do_erase_key(std::forward<K>(key), [&tmp](value_type&& val) {
						tmp = std::move(val);
					});
					return tmp;
				}
				void swap(table& other) noexcept(noexcept(std::is_nothrow_swappable_v<value_container_type> && std::is_nothrow_swappable_v<Hash> && std::is_nothrow_swappable_v<KeyEqual>)) {
					using std::swap;
					swap(other, *this);
				}
				template <typename Q = T, std::enable_if_t<is_map_v<Q>, bool> = true>
				auto at(key_type const& key) -> Q& {
					return do_at(key);
				}
				template <typename K,
						  typename Q = T,
						  typename H = Hash,
						  typename KE = KeyEqual,
						  std::enable_if_t<is_map_v<Q> && is_transparent_v<H, KE>, bool> = true>
				auto at(K const& key) -> Q& {
					return do_at(key);
				}
				template <typename Q = T, std::enable_if_t<is_map_v<Q>, bool> = true>
				auto at(key_type const& key) const -> Q const& {
					return do_at(key);
				}
				template <typename K,
						  typename Q = T,
						  typename H = Hash,
						  typename KE = KeyEqual,
						  std::enable_if_t<is_map_v<Q> && is_transparent_v<H, KE>, bool> = true>
				auto at(K const& key) const -> Q const& {
					return do_at(key);
				}
				template <typename Q = T, std::enable_if_t<is_map_v<Q>, bool> = true>
				auto operator[](Key const& key) -> Q& {
					return try_emplace(key).first->second;
				}
				template <typename Q = T, std::enable_if_t<is_map_v<Q>, bool> = true>
				auto operator[](Key&& key) -> Q& {
					return try_emplace(std::move(key)).first->second;
				}
				template <typename K,
						  typename Q = T,
						  typename H = Hash,
						  typename KE = KeyEqual,
						  std::enable_if_t<is_map_v<Q> && is_transparent_v<H, KE>, bool> = true>
				auto operator[](K&& key) -> Q& {
					return try_emplace(std::forward<K>(key)).first->second;
				}
				auto count(Key const& key) const -> size_t {
					return find(key) == end() ? 0 : 1;
				}
				template <class K, class H = Hash, class KE = KeyEqual, std::enable_if_t<is_transparent_v<H, KE>, bool> = true>
				auto count(K const& key) const -> size_t {
					return find(key) == end() ? 0 : 1;
				}
				auto find(Key const& key) -> iterator {
					return do_find(key);
				}
				auto find(Key const& key) const -> const_iterator {
					return do_find(key);
				}
				template <class K, class H = Hash, class KE = KeyEqual, std::enable_if_t<is_transparent_v<H, KE>, bool> = true>
				auto find(K const& key) -> iterator {
					return do_find(key);
				}
				template <class K, class H = Hash, class KE = KeyEqual, std::enable_if_t<is_transparent_v<H, KE>, bool> = true>
				auto find(K const& key) const -> const_iterator {
					return do_find(key);
				}
				auto contains(Key const& key) const -> bool {
					return find(key) != end();
				}
				template <class K, class H = Hash, class KE = KeyEqual, std::enable_if_t<is_transparent_v<H, KE>, bool> = true>
				auto contains(K const& key) const -> bool {
					return find(key) != end();
				}
				auto equal_range(Key const& key) -> std::pair<iterator, iterator> {
					auto it = do_find(key);
					return { it, it == end() ? end() : it + 1 };
				}
				auto equal_range(const Key& key) const -> std::pair<const_iterator, const_iterator> {
					auto it = do_find(key);
					return { it, it == end() ? end() : it + 1 };
				}
				template <class K, class H = Hash, class KE = KeyEqual, std::enable_if_t<is_transparent_v<H, KE>, bool> = true>
				auto equal_range(K const& key) -> std::pair<iterator, iterator> {
					auto it = do_find(key);
					return { it, it == end() ? end() : it + 1 };
				}
				template <class K, class H = Hash, class KE = KeyEqual, std::enable_if_t<is_transparent_v<H, KE>, bool> = true>
				auto equal_range(K const& key) const -> std::pair<const_iterator, const_iterator> {
					auto it = do_find(key);
					return { it, it == end() ? end() : it + 1 };
				}
				auto bucket_count() const noexcept -> size_t {
					return m_num_buckets;
				}
				static constexpr auto max_bucket_count() noexcept -> size_t {
					return max_size();
				}
				[[nodiscard]] auto load_factor() const -> float {
					return bucket_count() ? static_cast<float>(size()) / static_cast<float>(bucket_count()) : 0.0F;
				}
				[[nodiscard]] auto max_load_factor() const -> float {
					return m_max_load_factor;
				}
				void max_load_factor(float ml) {
					m_max_load_factor = ml;
					if (m_num_buckets != max_bucket_count()) {
						m_max_bucket_capacity = static_cast<value_idx_type>(static_cast<float>(bucket_count()) * max_load_factor());
					}
				}
				void rehash(size_t count) {
					count = (std::min)(count, max_size());
					auto shifts = calc_shifts_for_size((std::max)(count, size()));
					if (shifts != m_shifts) {
						m_shifts = shifts;
						deallocate_buckets();
						m_values.shrink_to_fit();
						allocate_buckets_from_shift();
						clear_and_fill_buckets_from_values();
					}
				}
				void reserve(size_t capa) {
					capa = (std::min)(capa, max_size());
					if constexpr (has_reserve<value_container_type>) {
						m_values.reserve(capa);
					}
					auto shifts = calc_shifts_for_size((std::max)(capa, size()));
					if (0 == m_num_buckets || shifts < m_shifts) {
						m_shifts = shifts;
						deallocate_buckets();
						allocate_buckets_from_shift();
						clear_and_fill_buckets_from_values();
					}
				}
				auto hash_function() const -> hasher {
					return m_hash;
				}
				auto key_eq() const -> key_equal {
					return m_equal;
				}
				[[nodiscard]] auto values() const noexcept -> value_container_type const& {
					return m_values;
				}
				friend auto operator==(table const& a, table const& b) -> bool {
					if (&a == &b) {
						return true;
					}
					if (a.size() != b.size()) {
						return false;
					}
					for (auto const& b_entry : b) {
						auto it = a.find(get_key(b_entry));
						if constexpr (is_map_v<T>) {
							if (a.end() == it || !(b_entry.second == it->second)) {
								return false;
							}
						}
						else {
							if (a.end() == it) {
								return false;
							}
						}
					}
					return true;
				}
				friend auto operator!=(table const& a, table const& b) -> bool {
					return !(a == b);
				}
			};
		} // namespace detail
		ANKERL_UNORDERED_DENSE_EXPORT template <class Key,
												class T,
												class Hash = hash<Key>,
												class KeyEqual = std::equal_to<Key>,
												class AllocatorOrContainer = std::allocator<std::pair<Key, T>>,
												class Bucket = bucket_type::standard>
		using map = detail::table<Key, T, Hash, KeyEqual, AllocatorOrContainer, Bucket, false>;
		ANKERL_UNORDERED_DENSE_EXPORT template <class Key,
												class T,
												class Hash = hash<Key>,
												class KeyEqual = std::equal_to<Key>,
												class AllocatorOrContainer = std::allocator<std::pair<Key, T>>,
												class Bucket = bucket_type::standard>
		using segmented_map = detail::table<Key, T, Hash, KeyEqual, AllocatorOrContainer, Bucket, true>;
		ANKERL_UNORDERED_DENSE_EXPORT template <class Key,
												class Hash = hash<Key>,
												class KeyEqual = std::equal_to<Key>,
												class AllocatorOrContainer = std::allocator<Key>,
												class Bucket = bucket_type::standard>
		using set = detail::table<Key, void, Hash, KeyEqual, AllocatorOrContainer, Bucket, false>;
		ANKERL_UNORDERED_DENSE_EXPORT template <class Key,
												class Hash = hash<Key>,
												class KeyEqual = std::equal_to<Key>,
												class AllocatorOrContainer = std::allocator<Key>,
												class Bucket = bucket_type::standard>
		using segmented_set = detail::table<Key, void, Hash, KeyEqual, AllocatorOrContainer, Bucket, true>;
#if defined(ANKERL_UNORDERED_DENSE_PMR)
		namespace pmr {
			ANKERL_UNORDERED_DENSE_EXPORT template <class Key,
													class T,
													class Hash = hash<Key>,
													class KeyEqual = std::equal_to<Key>,
													class Bucket = bucket_type::standard>
			using map =
				detail::table<Key, T, Hash, KeyEqual, ANKERL_UNORDERED_DENSE_PMR::polymorphic_allocator<std::pair<Key, T>>, Bucket, false>;
			ANKERL_UNORDERED_DENSE_EXPORT template <class Key,
													class T,
													class Hash = hash<Key>,
													class KeyEqual = std::equal_to<Key>,
													class Bucket = bucket_type::standard>
			using segmented_map =
				detail::table<Key, T, Hash, KeyEqual, ANKERL_UNORDERED_DENSE_PMR::polymorphic_allocator<std::pair<Key, T>>, Bucket, true>;
			ANKERL_UNORDERED_DENSE_EXPORT template <class Key,
													class Hash = hash<Key>,
													class KeyEqual = std::equal_to<Key>,
													class Bucket = bucket_type::standard>
			using set = detail::table<Key, void, Hash, KeyEqual, ANKERL_UNORDERED_DENSE_PMR::polymorphic_allocator<Key>, Bucket, false>;
			ANKERL_UNORDERED_DENSE_EXPORT template <class Key,
													class Hash = hash<Key>,
													class KeyEqual = std::equal_to<Key>,
													class Bucket = bucket_type::standard>
			using segmented_set =
				detail::table<Key, void, Hash, KeyEqual, ANKERL_UNORDERED_DENSE_PMR::polymorphic_allocator<Key>, Bucket, true>;
		} // namespace pmr
#endif
	} // namespace ANKERL_UNORDERED_DENSE_NAMESPACE
} // namespace ankerl::unordered_dense
namespace std {
	ANKERL_UNORDERED_DENSE_EXPORT template <class Key,
											class T,
											class Hash,
											class KeyEqual,
											class AllocatorOrContainer,
											class Bucket,
											class Pred,
											bool IsSegmented>
	auto erase_if(ankerl::unordered_dense::detail::table<Key, T, Hash, KeyEqual, AllocatorOrContainer, Bucket, IsSegmented>& map,
				  Pred pred) -> size_t {
		using map_t = ankerl::unordered_dense::detail::table<Key, T, Hash, KeyEqual, AllocatorOrContainer, Bucket, IsSegmented>;
		auto const old_size = map.size();
		auto idx = old_size;
		while (idx) {
			--idx;
			auto it = map.begin() + static_cast<typename map_t::difference_type>(idx);
			if (pred(*it)) {
				map.erase(it);
			}
		}
		return old_size - map.size();
	}
} // namespace std
MR_NAMESPACE_BEGIN
template <typename T, typename Y>
using unordered_map = ankerl::unordered_dense::map<T, Y>;
template <typename T>
using unordered_set = ankerl::unordered_dense::set<T>;
template <typename T>
using hash = ankerl::unordered_dense::hash<T>;
MR_NAMESPACE_END
#endif
#endif