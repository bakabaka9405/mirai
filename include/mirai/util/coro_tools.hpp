#include <mirai/pch.hpp>
#include <mirai/util/range.hpp>
#include <mirai/util/coro.hpp>
MR_NAMESPACE_BEGIN
generator<ll> irange_coro(ll l, ll r) {
	for (ll i = l; i < r; ++i) co_yield i;
}
generator<ll> irange_coro(ll l, ll r, ll step) {
	for (ll i = l; i < r; i += step) co_yield i;
}

MR_NAMESPACE_END