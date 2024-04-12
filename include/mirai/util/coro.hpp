#pragma once
#include <mirai/pch.hpp>
#include <mirai/util/debug.hpp>
MR_NAMESPACE_BEGIN

template <typename T>
class generator {
public:
	struct promise_type;
	using handle_type = std::coroutine_handle<promise_type>;
	struct promise_type {
		using pointer = std::add_pointer_t<T>;
		pointer value_;
		auto co() noexcept { return handle_type::from_promise(*this); }
		auto get_return_object() noexcept { return generator(co()); }
		std::suspend_always initial_suspend() noexcept { return {}; }
		std::suspend_always final_suspend() noexcept { return {}; }
		void unhandled_exception() noexcept { std::terminate(); }
		std::suspend_always yield_value(T& value) noexcept {
			value_ = std::addressof(value);
			return {};
		}
		std::suspend_always yield_value(T&& value) noexcept {
			value_ = std::addressof(value);
			return {};
		}
		void return_void() noexcept {}

		decltype(auto) value() noexcept { return *value_; }
	};

	explicit generator(handle_type h) noexcept
		: h_(h) {
		debug("generator constructed");
	}
	generator(const generator&) = delete;
	constexpr generator(generator&& other) noexcept
		: h_(std::exchange(other.h_, {})) {
		debug("generator moved");
	}
	~generator() noexcept {
		if (h_) h_.destroy();
		debug("generator destroyed");
	}

	struct iterator {
		using iterator_category = std::input_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = std::remove_reference_t<T>;
		using reference = std::add_lvalue_reference_t<T>;
		using pointer = std::add_pointer_t<T>;

		bool operator!=(auto&&) { return !h.done(); }
		void operator++() { h(); }
		value_type& operator*() { return h.promise().value(); }

		handle_type h;
	};

	auto end() const { return std::default_sentinel; }
	auto begin() {
		auto it = iterator{ h_ };
		if (!begin_) {
			++it;
			begin_ = true;
		}
		return it;
	}

private:
	handle_type h_;
	bool begin_ = false;
};
MR_NAMESPACE_END