#include <algorithm>
#include <cmath>
#include <functional>
#include <vector>

using std::vector;
// if size of vector is <= this, don't recurse
const size_t kBaseCase = 32;

namespace {
template <class T>
class FunnelTree {
	public:
		FunnelTree *left = nullptr, *right = nullptr;
		bool done = false;
		// the output buffer for this node; constbuffer is for leaves
		T *buffer;
		const T *constbuffer;
		size_t start = 0, end, cap;

		T get() {
			return (constbuffer == nullptr) ? buffer[start] : constbuffer[start];
		}
		T take() {
			T res = get();
			start++;
			return res;
		}

		template <class Comp>
		void fill(Comp comp);
};

// assumption: this can only be called when start = end
template <class T>
template <class Comp>
void FunnelTree<T>::fill(Comp comp) {
	// we are in a leaf, or in a node whose children are both done
	if ((left == nullptr && right == nullptr) || (left->done && right->done)) {
		done = true;
		return;
	}
	start = 0;
	end = 0;
	while (end < cap) {
		if (left->start == left->end)
			left->fill(comp);
		if (right->start == right->end)
			right->fill(comp);

		if (left->done && right->done) break;
		if (left->done)
			buffer[end++] = right->take();
		else if (right->done)
			buffer[end++] = left->take();
		else {
			T a = left->get(), b = right->get();
			if (comp(a, b)) buffer[end++] = left->take();
			else buffer[end++] = right->take();
		}
	}
}

void calculate_bufsizes_(vector<size_t>& v, int l, int r, size_t size) {
	// take square root of size
	size = pow(size, 0.5) + 1;
	if (r - l <= 2) {
		for (; l < r; l++) v[l] = size;
		return;
	}
	int m = (l + r) / 2;
	v[m] = size;
	calculate_bufsizes_(v, l, m, size);
	calculate_bufsizes_(v, m + 1, r, size);
}

vector<size_t> calculate_bufsizes(int depth, size_t initial_size) {
	vector<size_t> res(depth);
	res[0] = initial_size;
	calculate_bufsizes_(res, 1, depth, initial_size);
	return res;
}

template <class T>
FunnelTree<T>* create_tree_(int d, const vector<size_t>& depth_to_size,
							const vector<vector<T>>& lists, int l, int r,
							T* buffer, size_t *buf_used) {
	auto *res = new FunnelTree<T>;
	// this node represents a single thing; leaf node
	if (l == r + 1) {
		res->constbuffer = lists[l].data();
		res->end = lists[l].size();
		res->cap = lists[l].size();
		return res;
	}
	// since it's un-populated
	res->end = 0;
	res->cap = depth_to_size[d];

	res->buffer = buffer + (*buf_used);
	*buf_used += res->cap;
	int m = (l + r) / 2;
	res->left = create_tree_(d+1, depth_to_size, lists, l, m, buffer, buf_used);
	res->right = create_tree_(d+1, depth_to_size, lists, m, r, buffer, buf_used);
}

template <class T>
FunnelTree<T>* create_tree(const vector<vector<T>>& lists) {
	size_t k = lists.size(), tot_size = 0;
	for (auto& l : lists)
		tot_size += l.size();

	int max_depth = 1;
	while ((1 << max_depth) < k)
		max_depth++;

	const vector<size_t> bufsizes = calculate_bufsizes(max_depth + 1, tot_size);
	size_t tot_buf_size = 0;
	for (int i = 0; i < max_depth; i++)
		tot_buf_size += (1<<i) * bufsizes[i];
	// total number of nodes is always 2k-1; subtract (2^max_depth - 1)
	tot_buf_size += (2*k - (1 << max_depth)) * bufsizes[max_depth];
	T* buffer = new T[tot_buf_size];

	size_t buf_used = 0;
	return create_tree_(0, bufsizes, lists, 0, k, buffer, &buf_used);
}
}  // namespace

template <class RandomIt, class Comp>
vector<typename RandomIt::value_type> funnelsort(RandomIt first, RandomIt last, Comp comp);

template <class RandomIt>
vector<typename RandomIt::value_type> funnelsort(RandomIt first, RandomIt last) {
	return funnelsort(first, last, std::greater<typename RandomIt::value_type>());
}

template <class T, class Comp>
vector<T> merge(const vector<vector<T>>& lists, Comp comp);

template <class RandomIt, class Comp>
vector<typename RandomIt::value_type> funnelsort(RandomIt first, RandomIt last, Comp comp) {
	typedef typename RandomIt::value_type T;
	size_t size = last - first;
	if (size <= kBaseCase) {
		vector<T> arr_copy(first, last);
		std::sort(arr_copy.begin(), arr_copy.end());
		return arr_copy;
	}

	int szCubeRoot = pow(size, 1.0/3.0);
	vector<vector<T>> recursive_results(szCubeRoot);
	int loSize = size / szCubeRoot;
	// x * loSize + (szCubeRoot - x) * (loSize + 1) = size
	// szCubeRoot * loSize + szCubeRoot - x = size
	// x = szCubeRoot * (loSize + 1) - size
	int numLoSize = szCubeRoot * (loSize + 1) - size;
	for (int i = 0; i < szCubeRoot; i++) {
		int curSize = (i < numLoSize) ? loSize : loSize + 1;
		recursive_results[i] = funnelsort(first, first + curSize, comp);
		first += curSize;
	}
	return merge(recursive_results, comp);
}

template <class T, class Comp>
vector<T> merge(const vector<vector<T>>& lists, Comp comp) {
	auto root = create_tree(lists);
	root->fill(comp);
	vector<T> res(root->buffer, root->buffer + root->cap);
	return res;
}
