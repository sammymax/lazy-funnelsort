#include <functional>
#include <vector>
#include <iostream>
#include <time.h>
#include <ctime>
#include <chrono>

#include "funnelsort.h"
#include "cpucounters.h"

using std::cout;
using std::cin;
using std::endl;
using namespace std::chrono;

int main(int argc, char** argv) {
	int arr_size = atoi(argv[1]);
	int method = atoi(argv[2]); //0 for funnelsort, 1 for normal
	//cout << arr_size << endl << method;

	high_resolution_clock::time_point start = high_resolution_clock::now();
	//clock_t start = clock();

	std::vector<int> v(arr_size);
	for (int &x : v) x = rand();
	
	//for (int &x: v) {
	//	cout << x << endl;
	//}
	
	std::vector<int> res(arr_size);
	
	high_resolution_clock::time_point end = high_resolution_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(end-start);

	cout << "Milliseconds for prep: " << time_span.count() << endl;
	//cout << "Some output"; 
	//time_t start_time;
	//time_t end_time;

	//time(&start_time);

	//start = clock();
	start = high_resolution_clock::now();
	PCM * m = PCM::getInstance();
	m->resetPMU();
	PCM::ErrorCode returnResult = m->program();
	if (returnResult != PCM::Success){
		m->cleanup();
		std::cerr << "Intel's PCM couldn't start" << std::endl;
		std::cerr << "Error code: " << returnResult << std::endl;
		exit(1);
	}
	SystemCounterState before_sstate = getSystemCounterState();

	if (method == 0) {
		cout << "Using funnelsort" << endl;
		res = funnelsort(v.begin(), v.end());
	} else {
		cout << "Using std sort" << endl;
		std::stable_sort(v.begin(), v.end());
	}

	end = high_resolution_clock::now();
	SystemCounterState after_sstate = getSystemCounterState();
	time_span = duration_cast<duration<double>>(end-start);
	m->cleanup();
	cout << "Sort time: " <<  time_span.count() << endl;
	cout << "L3 miss ratio: " << getL3CacheHitRatio(before_sstate,after_sstate) << endl;
	cout << "L3 hits: " << getL3CacheHits(before_sstate,after_sstate) << endl;
	cout << "L3 miss ratio: " << getL3CacheMisses(before_sstate,after_sstate) << endl;

	cout << "L2 hits: " << getL2CacheHitRatio(before_sstate,after_sstate) << endl;
	cout << "L2 hits: " << getL2CacheHits(before_sstate,after_sstate) << endl;
	cout << "L2 misses: " << getL2CacheMisses(before_sstate,after_sstate) << endl;

	//cout << "Milliseconds: " << clock() - start << endl;
	//time(&end_time);

	//double seconds = difftime(end_time, start_time);
	//cout << "Seconds: " << seconds << endl;


}
