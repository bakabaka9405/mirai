#include <mirai/pch.hpp>
namespace mirai {
	template <typename T>
	struct segment_tree_node {
		T val;
		segment_tree_node* left;
		segment_tree_node* right;
		segment_tree_node(T val, segment_tree_node* left = nullptr, segment_tree_node* right = nullptr)
			: val(val), left(left), right(right) {}
	};
	template <typename T, size_t size, typename Node = segment_tree_node<T>, typename Allocator = std::allocator<Node>>
	class persistent_array {
	private:
		Node* root;
		inline static Allocator alloc;
		T get(Node* x, ll pos, ll L, ll R) {
			while (L != R) {
				ll M = (L + R) / 2;
				if (pos <= M) {
					x = x->left;
					R = M;
				}
				else {
					x = x->right;
					L = M + 1;
				}
			}
			return x->val;
		}
		Node* set(Node* x, ll pos, T val, ll L, ll R) {
			if (L == R) return new Node(val);
			ll M = (L + R) / 2;
			if (pos <= M)
				return new Node(x->val, set(x->left, pos, val, L, M), x->right);
			else
				return new Node(x->val, x->left, set(x->right, pos, val, M + 1, R));
		}
		template <size_t N>
		Node* set(ll (&arr)[N], ll L, ll R) {
			if (L == R) return new Node(arr[L]);
			ll M = (L + R) / 2;
			return new Node(0, set(arr, L, M), set(arr, M + 1, R));
		}
		persistent_array(Node* root)
			: root(root) {}

	public:
		persistent_array()
			: root(nullptr) {}
		persistent_array(const persistent_array& x)
			: root(x.root) {}
		persistent_array(persistent_array&& x)
			: root(x.root) {
			x.root = nullptr;
		}
		persistent_array& operator=(const persistent_array& x) {
			if (this == &x) return *this;
			root = x.root;
			return *this;
		}
		persistent_array& operator=(persistent_array&& x) {
			if (this == &x) return *this;
			root = x.root;
			x.root = nullptr;
			return *this;
		}
		T get(ll i) {
			return get(root, i, 0, size);
		}
		persistent_array set(ll i, T val) {
			return persistent_array{ set(root, i, val, 0, size) };
		}
		template <size_t N>
		void set(T (&val)[N]) {
			root = set(val, 0, size);
		}
	};
} // namespace mirai