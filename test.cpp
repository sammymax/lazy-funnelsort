#include <functional>
#include <vector>
#include <iostream>

#include "funnelsort.h"

using std::cout;

int main() {
	for (int i = 1; i < 1000; i++) {
		std::vector<int> v(i);
		for (int &x : v) x = rand();
		auto res = funnelsort(v.begin(), v.end());
		sort(v.begin(), v.end());

		bool ok = true;
		for (int j = 0; j < i; j++)
			if (res[j] != v[j]) ok = false;

		if (!ok) {
			cout << "failed! ours/theirs:\n";
			for (auto x : res) cout << x << ' ';
			cout << '\n';
			for (auto x : v) cout << x << ' ';
			cout << '\n';
			break;
		}
	}
}
