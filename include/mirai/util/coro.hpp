#include <mirai/pch.hpp>
MR_NAMESPACE_BEGIN

template <typename T>
class generator {
public:
	struct promise_type;
	using handle_type = std::coroutine_handle<promise_type>;
	struct promise_type {
		using pointer = std::add_pointer_t<T>;
		pointer value_;

		auto co() mr_noexcept { return handle_type::from_promise(*this); }
		auto get_return_object() mr_noexcept { return generator(co()); }
		std::suspend_always initial_suspend() mr_noexcept { return {}; }
		std::suspend_always final_suspend() noexcept { return {}; }
		void unhandled_exception() mr_noexcept { throw; }
		std::suspend_always yield_value(T&& value) mr_noexcept {
			value_ = std::addressof(value);
			return {};
		}
		void return_void() mr_noexcept {}

		decltype(auto) value() mr_noexcept { return *value_; }
	};

	explicit generator(handle_type h) mr_noexcept : h_(h) {}
	~generator() mr_noexcept {
		if (h_) h_.destroy();
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