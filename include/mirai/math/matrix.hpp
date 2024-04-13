#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/proxy.hpp>
#include <mirai/math/vec2.hpp>
MR_NAMESPACE_BEGIN
template <typename T, size_t n, size_t m, class add_proxy>
struct matrix {
	T v[n][m];
	constexpr matrix() mr_noexcept : v{ { 0 } } {}
	explicit constexpr matrix(const T (&&val)[n][m]) mr_noexcept {
		for (size_t i = 0; i < n; i++) {
			for (size_t j = 0; j < m; j++) v[i][j] = val[i][j];
		}
	}
	constexpr matrix(const std::initializer_list<T>& val) mr_noexcept
		: matrix() {
		for (T* p = &v[0][0]; auto&& v : val) *p++ = std::move(v);
	}
	constexpr matrix(const matrix& rt) mr_noexcept : matrix() {
		T* p = &v[0][0];
		const T* q = &rt.v[0][0];
		for (auto i = n * m; i; --i) *p++ = *q++;
	}

	MR_NODISCARD constexpr static matrix I() mr_noexcept {
		static_assert(n == m);
		matrix res;
		for (size_t i = 0; i < n; i++) res.v[i][i] = 1;
		return res;
	}

	MR_NODISCARD constexpr matrix operator+(const matrix& rt) const mr_noexcept {
		matrix res = *this;
		for (size_t i = n; i; i--) {
			for (size_t j = m; j; j--) {
				v[i][j] = add_proxy::work(v[i][j], rt.v[i][j]);
			}
		}
	}

	template <std::size_t t>
	MR_NODISCARD constexpr matrix<T, n, t, add_proxy> operator*(const matrix<T, m, t, add_proxy>& rt) const mr_noexcept {
		matrix<T, n, t, add_proxy> res;
		for (ll i = 0; i < n; ++i)
			for (ll k = 0; k < t; ++k) {
				ll r = v[i][k];
				for (ll j = 0; j < m; ++j)
					res.v[i][j] = add_proxy::work(res.v[i][j], rt.v[k][j] * r);
			}
		return res;
	}

	T* operator[](ll x) mr_noexcept { return v[x]; }

	constexpr T* operator[](ll x) const mr_noexcept { return v[x]; }

	MR_NODISCARD T& operator[](const vec2i& pos) mr_noexcept { return v[pos.x][pos.y]; }

	MR_NODISCARD constexpr T operator[](const vec2i& pos) const mr_noexcept { return v[pos.x][pos.y]; }

	constexpr bool operator==(const matrix& rt) const mr_noexcept {
		for (ll i = 0; i < n; i++)
			for (ll j = 0; j < m; j++)
				if (!equal_to(v[i][j], rt.v[i][j])) return false;
		return true;
	}

	constexpr bool operator!=(const matrix& rt) const mr_noexcept {
		return this->operator==(rt);
	}

	MR_NODISCARD constexpr matrix<T, m, n, add_proxy> transpose() const mr_noexcept {
		matrix<T, m, n, add_proxy> res;
		for (ll i = 0; i < n; i++)
			for (ll j = 0; j < m; j++) res[j][i] = res[i][j];
		return res;
	}
};

template <typename T, std::size_t n, std::size_t m, class add_proxy>
std::ostream& operator<<(std::ostream& out, const matrix<T, n, m, add_proxy>& mat) mr_noexcept {
	for (auto&& r : mat.v) {
		for (auto&& x : r) out << x << " ";
		out << endl;
	}
	return out;
}

template <typename T, std::size_t n, class add_proxy>
matrix<T, n, n, add_proxy> fastpow(matrix<T, n, n, add_proxy> x, ull k) mr_noexcept {
	matrix<T, n, n, add_proxy> res = matrix<T, n, n, add_proxy>::I();
	while (k) {
		if (k & 1u) res = res * x;
		x = x * x;
		k >>= 1u;
	}
	return res;
}
MR_NAMESPACE_END