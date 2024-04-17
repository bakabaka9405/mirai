#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/hash.hpp>
#if MR_USE_GCC
#define ANKERL_UNORDERED_DENSE_PACK(decl) decl __attribute__((__packed__))
#elif MR_USE_MSVC
#define ANKERL_UNORDERED_DENSE_PACK(decl) __pragma(pack(push, 1)) decl __pragma(pack(pop))
#endif
#if MR_USE_MSVC
#define ANKERL_UNORDERED_DENSE_NOINLINE __declspec(noinline)
#else
#define ANKERL_UNORDERED_DENSE_NOINLINE __attribute__((noinline))
#endif
MR_NAMESPACE_BEGIN
namespace detail {
#if MR_HAVE_EXCEPTIONS
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
namespace detail {
	template <class Key,
			  class T,
			  class Hash,
			  class KeyEqual,
			  class AllocatorOrContainer,
			  class Bucket>
	class table : public std::conditional_t<is_map_v<T>, base_table_type_map<T>, base_table_type_set> {
		using underlying_value_type = typename std::conditional_t<is_map_v<T>, std::pair<Key, T>, Key>;
		using underlying_container_type = std::vector<underlying_value_type, AllocatorOrContainer>;

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
		MR_NODISCARD auto next(value_idx_type bucket_idx) const -> value_idx_type {
			return ANKERL_UNORDERED_DENSE_UNLIKELY(bucket_idx + 1U == m_num_buckets)
					   ? 0
					   : static_cast<value_idx_type>(bucket_idx + 1U);
		}
		MR_NODISCARD static constexpr auto at(bucket_pointer bucket_ptr, size_t offset) -> Bucket& {
			return *(bucket_ptr + static_cast<typename std::allocator_traits<bucket_alloc>::difference_type>(offset));
		}
		MR_NODISCARD static constexpr auto dist_inc(dist_and_fingerprint_type x) -> dist_and_fingerprint_type {
			return static_cast<dist_and_fingerprint_type>(x + Bucket::dist_inc);
		}
		MR_NODISCARD static constexpr auto dist_dec(dist_and_fingerprint_type x) -> dist_and_fingerprint_type {
			return static_cast<dist_and_fingerprint_type>(x - Bucket::dist_inc);
		}
		template <typename K>
		MR_NODISCARD constexpr auto mixed_hash(K const& key) const -> uint64_t {
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
		MR_NODISCARD constexpr auto dist_and_fingerprint_from_hash(uint64_t hash) const -> dist_and_fingerprint_type {
			return Bucket::dist_inc | (static_cast<dist_and_fingerprint_type>(hash) & Bucket::fingerprint_mask);
		}
		MR_NODISCARD constexpr auto bucket_idx_from_hash(uint64_t hash) const -> value_idx_type {
			return static_cast<value_idx_type>(hash >> m_shifts);
		}
		MR_NODISCARD static constexpr auto get_key(value_type const& vt) -> key_type const& {
			if constexpr (is_map_v<T>) {
				return vt.first;
			}
			else {
				return vt;
			}
		}
		template <typename K>
		MR_NODISCARD auto next_while_less(K const& key) const -> Bucket {
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
		MR_NODISCARD static constexpr auto calc_num_buckets(uint8_t shifts) -> size_t {
			return (std::min)(max_bucket_count(), size_t{ 1 } << (64U - shifts));
		}
		MR_NODISCARD constexpr auto calc_shifts_for_size(size_t s) const -> uint8_t {
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
		MR_NODISCARD auto is_full() const -> bool {
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
			if (auto it = find(key); MR_EXPECT(end() != it)) {
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
		MR_NODISCARD auto empty() const noexcept -> bool {
			return m_values.empty();
		}
		MR_NODISCARD auto size() const noexcept -> size_t {
			return m_values.size();
		}
		MR_NODISCARD static constexpr auto max_size() noexcept -> size_t {
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
			return do_erase_key(key, [](value_type&&) {});
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
		MR_NODISCARD auto bucket_count() const noexcept -> size_t {
			return m_num_buckets;
		}
		static constexpr auto max_bucket_count() noexcept -> size_t {
			return max_size();
		}
		MR_NODISCARD auto load_factor() const -> float {
			return bucket_count() ? static_cast<float>(size()) / static_cast<float>(bucket_count()) : 0.0F;
		}
		MR_NODISCARD auto max_load_factor() const -> float {
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
		MR_NODISCARD auto values() const noexcept -> value_container_type const& {
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
template <class Key,
		  class T,
		  class Hash = hash<Key>,
		  class KeyEqual = std::equal_to<Key>,
		  class AllocatorOrContainer = std::allocator<std::pair<Key, T>>,
		  class Bucket = bucket_type::standard>
using unordered_map = detail::table<Key, T, Hash, KeyEqual, AllocatorOrContainer, Bucket>;
template <class Key,
		  class Hash = hash<Key>,
		  class KeyEqual = std::equal_to<Key>,
		  class AllocatorOrContainer = std::allocator<Key>,
		  class Bucket = bucket_type::standard>
using unordered_set = detail::table<Key, void, Hash, KeyEqual, AllocatorOrContainer, Bucket>;
namespace pmr {
	template <class Key,
			  class T,
			  class Hash = hash<Key>,
			  class KeyEqual = std::equal_to<Key>,
			  class Bucket = bucket_type::standard>
	using unordered_map =
		detail::table<Key, T, Hash, KeyEqual, std::pmr::polymorphic_allocator<std::pair<Key, T>>, Bucket>;
	template <class Key,
			  class Hash = hash<Key>,
			  class KeyEqual = std::equal_to<Key>,
			  class Bucket = bucket_type::standard>
	using unordered_set = detail::table<Key, void, Hash, KeyEqual, std::pmr::polymorphic_allocator<Key>, Bucket>;
} // namespace pmr
MR_NAMESPACE_END
namespace std {
	template <class Key,
			  class T,
			  class Hash,
			  class KeyEqual,
			  class AllocatorOrContainer,
			  class Bucket,
			  class Pred>
	auto erase_if(mirai::detail::table<Key, T, Hash, KeyEqual, AllocatorOrContainer, Bucket>& map,
				  Pred pred) -> size_t {
		using map_t = mirai::detail::table<Key, T, Hash, KeyEqual, AllocatorOrContainer, Bucket>;
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