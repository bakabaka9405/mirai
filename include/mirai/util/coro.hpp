#pragma once
#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN

template <typename T>
class generator {
private:
	using Yielded = T&&;
	using Yielded_decvref = std::remove_cvref_t<Yielded>;

public:
	struct promise_type;
	using handle_type = std::coroutine_handle<promise_type>;
	struct promise_type {
		using pointer = std::add_pointer_t<Yielded>;
		pointer value_;
		auto get_return_object() noexcept { return generator{ handle_type::from_promise(*this) }; }
		std::suspend_always initial_suspend() noexcept { return {}; }
		std::suspend_always final_suspend() noexcept { return {}; }
		void unhandled_exception() noexcept { std::terminate(); }
		std::suspend_always yield_value(Yielded value) noexcept {
			value_ = std::addressof(value);
			return {};
		}
		auto yield_value(const Yielded_decvref& value) noexcept
			requires std::is_rvalue_reference_v<Yielded> && std::constructible_from<Yielded_decvref, const Yielded_decvref&>
		{
			struct copy_awaiter {
				Yielded_decvref src;
				pointer& dst;
				constexpr bool await_ready() noexcept { return false; }
				void await_suspend(handle_type) noexcept { dst = std::addressof(src); }
				constexpr void await_resume() const noexcept {}
			};
			return copy_awaiter{ value, value_ };
		}
		void return_void() noexcept {}

		decltype(auto) value() noexcept { return *value_; }
	};

	explicit generator(handle_type h) noexcept
		: h_(move(h)) {}

	generator(const generator&) = delete;
	generator& operator=(generator rt) noexcept {
		swap(h_, rt.h_);
		swap(begin_, rt.begin_);
		return *this;
	}
	constexpr generator(generator&& other) noexcept
		: h_(exchange(other.h_, nullptr)), begin_(exchange(other.begin_, false)) {
	}
	~generator() noexcept {
		if (h_) {
			h_.destroy();
		}
	}

	struct iterator {
		using iterator_category = std::input_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = std::remove_reference_t<T>;
		using reference = std::add_lvalue_reference_t<T>;
		using pointer = std::add_pointer_t<T>;

		friend bool operator==(const iterator& _i, std::default_sentinel_t)noexcept { return _i.h.done(); }
		iterator& operator++()noexcept {
			h();
			return *this;
		}
		void operator++(int) noexcept { h(); }
		value_type& operator*() noexcept {
			return h.promise().value();
		}

		handle_type h;
	};

	auto end() const noexcept { return std::default_sentinel; }
	auto begin() noexcept {
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