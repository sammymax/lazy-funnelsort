#include <functional>
#include <vector>
#include <iostream>

#include "funnelsort.h"

using std::cout;
using std::cin;
using std::endl;
int main(int argc, char** argv) {
	int arr_size = atoi(argv[1]);
	int method = atoi(argv[2]); //0 for funnelsort, 1 for normal
	//cout << arr_size << endl << method;

	std::vector<int> v(arr_size);
	for (int &x : v) x = rand();
	
	//for (int &x: v) {
	//	cout << x << endl;
	//}
	
	std::vector<int> res(arr_size);
	
	//cout << "Some output"; 
	if (method == 0) {
		cout << "Using funnelsort" << endl;
		res = funnelsort(v.begin(), v.end());
	} else {
		cout << "Using std sort" << endl;
		sort(v.begin(), v.end());
	}
}
