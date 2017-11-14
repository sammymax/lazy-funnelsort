#include <functional>
#include <vector>

#include "funnelsort.h"

int main() {
	funnelsort(std::vector<int>{12, 2}, std::greater<int>());
}
