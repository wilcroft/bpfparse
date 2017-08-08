#include "ternary.h"
#include <vector>
#include <iostream>

int main() {

	ternary t1;
	ternary t2(ternary::TERN_0);
	ternary t3(true);

	std::vector<ternary> v1;
	std::vector<ternary> v2;

	if (!(t1 != t2)) return -1;
	if (!(t1 == t2)) return -2;
	if (!(t1 != t3)) return -3;
	if (!(t1 != t3)) return -4;
	if (t2 == t3) return -5;

	v1.resize(2);
	v2.resize(2);
	if (v1 != v2) return -6;
	
	v1[0] = ternary::TERN_1;
	if (v1 != v2) return -7;

	v1[1] = false;
	if (v1 != v2) return -8;

	v2[0] = false;
	if (v1 == v2) return -9;

	return 0;
}