#include <algorithm>
#include <cmath>
#include <functional>
#include <vector>

using std::vector;
// if size of vector is <= this, don't recurse
const size_t kBaseCase = 32;

template <class RandomIt, class Comp>
vector<typename RandomIt::value_type> funnelsort(RandomIt first, RandomIt last, Comp comp);

template <class RandomIt>
vector<typename RandomIt::value_type> funnelsort(RandomIt first, RandomIt last) {
	return funnelsort(first, last, std::greater<typename RandomIt::value_type>());
}

template <class T, class Comp>
vector<T> merge(vector<vector<T>> lists, Comp comp);

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
vector<T> merge(vector<vector<T>> lists, Comp comp) {
}
