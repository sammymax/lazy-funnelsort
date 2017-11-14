#include <functional>
#include <vector>

#include "funnelsort.h"

int main() {
	std::vector<int> v {12, 2};
	funnelsort(v.begin(), v.end(), std::greater<int>());
	funnelsort(v.begin(), v.end());
}
