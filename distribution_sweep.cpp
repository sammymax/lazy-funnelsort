#include <functional>
#include <vector>
#include <iostream>
#include <limits>
#include "funnelsort.h"

using std::cout;
using std::cin;
using std::endl;

class point {
	public:
		int x, y, z;
		bool marked;
		bool true_marked;
};

void pointprint(point p) {
	cout << "pointprint: " << p.x << " " << p.y << " " << p.z << " " << endl;
}

class sort_by_z { 
	//class provided to lazy funnelsort to sort points in increasing order by z coordinate
	public:
	bool operator() (point& p1, point& p2) {
		//Arguments: p1 from left buffer, p2 from right buffer
		//Output: true if merge should take from right buffer
		//	      false if merge should take from left buffer

		//Purpose: sorts on z coordinate in increasing order
		//should take from whatever buffer has point with lower z coordinate
		//breaks ties first on x, then y (check this is legit...)

		if (p1.z == p2.z) {
			if (p1.x == p2.x) {
				return (p1.y > p2.y);
			}
			return (p1.x > p2.x);
		}

		return (p1.z > p2.z); //returns true when left buffer has higher z coordinate i.e. right buffer should be taken
	}

	void right_take_update(point& p1) {
		//Update information of point taken from right buffer in merge step
		return;
	}

	void left_take_update(point& p1) {
		//Update information of point taken from left buffer in merge step
		return;
	}

	static bool base_comp(point& p1, point& p2) {
		//base comparator used for std::sort, should return true when p1 < p2 by z-coord
		//std::sort sorts in increasing order

		if (p1.z == p2.z) {
			if (p1.x == p2.x) {
				return (p1.y < p2.y);
			}
			return (p1.x < p2.x);
		}

		return (p1.z < p2.z); //returns true when p1 has lower z coordinate, i.e. p1 < p2
	}

	std::vector<point> base_case(std::vector<point> arr) {	
		//base case to take by lazy funnelsort
		std::sort(arr.begin(), arr.end(), base_comp);
		return arr;
	}
};

class maximal_merge {
	//Purpose: class provided to lazy funnelsort to perform merging of points in distribution sweeping soln to 3D maximal points
	
	//Space for auxillary info needed by merger at each node to perform merge and updates
	int auxinfo = std::numeric_limits<int>::lowest();
	
	public:
	bool operator() (point& p1, point& p2) {
		//Purpose: choose whichever point has higher x-coordinate to merge into buffer
		//		   also update auxinfo to keep track of highest y-coordinate seen from right buffer
		
		//DEBUG output
		//cout << "left buffer: ";
		//pointprint(p1);
		//cout << "right buffer: ";
		//pointprint(p2);
		//cout << "auxinfo " << auxinfo << endl;

		if (p1.x <= p2.x) { //right buffer has higher x-coordinate, so choose it and update auxinfo
			auxinfo = std::max(auxinfo, p2.y);
			//cout << "right buffer chosen"<< endl;
			return true;
		} else {
			//cout << "left buffer chosen" << endl;
			return false;
		}
	}

	void right_take_update(point& p1) {
		return;
	}

	void left_take_update(point& p1) {
		//Purpose: when point from left buffer is taken, change it's marking if necesary

		//DEBUG output
		//cout << "left take update" << endl;
		//pointprint(p1);
		//cout << p1.marked << endl;
		//cout << auxinfo << endl;
		if (p1.marked) {
			if (p1.y <= auxinfo) {
				//cout << "p1 just marked false i.e. 0" << endl;
				p1.marked = false;
			}
		}
	}
	
	static bool base_comp_x(point& p1, point& p2) {
		//Purpose: comparator for std::sort for base case, when we want to sort by decreasing x-coordinate

		if (p1.x == p2.x) {
			if (p1.y == p2.y) {
				return (p1.z > p2.z);
			}
			return (p1.y > p2.y);
		}

		return (p1.x > p2.x);
	}

	std::vector<point> base_case(std::vector<point> arr) {
		//make correct markings in base case since base case can have size up to 8
		//then sort by decreasing x-coordinate

		for (int i=0; i < arr.size(); i++) {
			bool not_dominated = true;
			point& p = arr[i];
			for (int j = 0; j < arr.size(); j++) {
				if (i != j) {
					point q = arr[j];
					if ((q.x >= p.x) && (q.y >= p.y) && (q.z >= p.z)) {not_dominated = false;}
				}	
			}
			p.marked = not_dominated;
		}

		std::sort(arr.begin(), arr.end(), base_comp_x);
		return arr;
	}

};

int main() {

	int N = 11;

	std::vector<point> all_points;
	int xcoords [N] = {1, 2, 3, 4, 5, 7, 8, 6, 10, 11, 9};
	int ycoords [N] = {1, 2, 5, 4, 3, 7, 6, 8, 10, 9, 11};
	int zcoords [N] = {1, 2, 3, 5, 4, 6, 7, 8, 9, 10, 11};

	for (int i=0; i < N; i++) {
		point new_p;
		new_p.x = xcoords[i];
		new_p.y = ycoords[i];
		new_p.z = zcoords[i];
		new_p.marked = true;
		all_points.push_back(new_p);
	}

	//brute-force check--p.true_marked holds result
	for (int i=0; i < all_points.size(); i++) {
		bool not_dominated = true;
		point& p = all_points[i];
		for (int j = 0; j < all_points.size(); j++) {
			if (i != j) {
				point q = all_points[j];
				if ((q.x >= p.x) && (q.y >= p.y) && (q.z >= p.z)) { not_dominated = false;}
			}	
		}
		p.true_marked = not_dominated;
	}

	std::vector<point> all_points_z_sorted = funnelsort(all_points.begin(), all_points.end(), sort_by_z());
	
	cout << "Points after z sort" << endl;
	for (point p: all_points_z_sorted) {
		pointprint(p);
	}

	std::vector<point> all_points_marked = funnelsort(all_points_z_sorted.begin(), all_points_z_sorted.end(), maximal_merge());
	
	cout << "Points after marking and true marks" << endl;

	for (point p: all_points_marked) {
		
		bool true_status;
		for (point prime: all_points) { //find original point
			if (prime.x == p.x && prime.y == p.y && prime.z == p.z) {
				true_status = prime.true_marked;
				break;
			}
		}
		
		pointprint(p);
		if (p.marked == true_status) cout << "Marking is right!" << endl;
		else cout << "ALERT: Marking is WRONG" << endl;
	}
}
