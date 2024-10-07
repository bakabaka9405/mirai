#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN
template <typename Iterator>
void generate_recurrence_sequence(Iterator begin, Iterator end, auto&& v0, auto&& nxt) {
	*begin = v0;
	for (auto it = std::next(begin), lst = begin; it != end; ++it, ++lst) *it = nxt(*lst);
}

inline void generate_inv_sequence(std::random_access_iterator auto it,
								  size_t n, ull P) mr_noexcept {
	it[1] = 1;
	auto p = it + 2;
	for (size_t i = 2; i <= n; ++i, ++p) {
		*p = (P - P / i) * it[P % i] % P;
	}
}

inline void generate_prime_sequence(std::random_access_iterator auto it,
									vector<ll>& primes, ll n) mr_noexcept {
	for (ll i = 2; i <= n; i++) {
		if (!it[i]) primes.push_back(i);
		for (auto p : primes) {
			if (i * p > (ll)n) break;
			it[i * p] = p;
			if (i % p == 0) break;
		}
	}
}

inline void generate_minimal_divisor_sequence(std::random_access_iterator auto not_prime,
											  std::random_access_iterator auto min_div,
											  vector<ll>& primes, ll n) {
	for (ll i = 2; i <= n; i++) {
		if (!not_prime[i]) {
			primes.push_back(i);
			min_div[i] = i;
		}
		for (auto p : primes) {
			if (i * p > (ll)n) break;
			not_prime[i * p] = p;
			min_div[i * p] = p;
			if (i % p == 0) break;
		}
	}
}

inline void generate_eura_sequence(std::random_access_iterator auto min_div, std::random_access_iterator auto eura, ll n) mr_noexcept {
	for (ll i = 2; i <= n; i++) {
		if (min_div[i] == i) eura[i] = i - 1;
		else eura[i] = eura[i / min_div[i]] * eura[min_div[i]];
	}
}

inline void generate_divisor_count_sequence(std::random_access_iterator auto not_prime,
											std::random_access_iterator auto count,
											std::random_access_iterator auto unique_count,
											vector<ll>& primes, ll n) mr_noexcept {
	count[1] = unique_count[1] = 1;
	for (ll i = 2; i <= n; i++) {
		if (!not_prime[i]) {
			primes.push_back(i);
			count[i] = 2, unique_count[i] = 1;
		}
		for (auto p : primes) {
			if (i * p > n) break;
			not_prime[i * p] = true;
			if (i % p == 0) {
				unique_count[i * p] = unique_count[i] + 1;
				count[i * p] = count[i] / (unique_count[i] + 1) * (unique_count[i] + 2);
				break;
			}
			unique_count[i * p] = 1;
			count[i * p] = count[i] * 2;
		}
	}
}
MR_NAMESPACE_END